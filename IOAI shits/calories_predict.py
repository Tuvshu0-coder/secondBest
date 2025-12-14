"""Competition-grade calories prediction pipeline

This script is a complete, modular pipeline that:
- Loads and cleans the data (robustly, preserving information)
- Engineers predictive features (BMI, interactions, ratios, temperature deviation)
- Detects and removes simple data leaks
- Runs Optuna hyperparameter tuning for XGBoost, LightGBM, CatBoost,
  RandomForest and ElasticNet (when available)
- Builds a stacking ensemble from top models
- Retrains the selected final model on 100% training data
- Outputs `submission.csv` with `User_ID, Calories` rounded to 2 decimals

Notes:
- The script tries to import optional libraries (xgboost, lightgbm, catboost,
  optuna). If a package is missing it will skip that model and continue.
- Tuning can be time-consuming; adjust `N_TRIALS` and `cv_folds` as needed.
"""

from pathlib import Path
import warnings
import argparse
import numpy as np
import pandas as pd

warnings.filterwarnings("ignore")

# Core ML libraries
from sklearn.model_selection import KFold, cross_val_score
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.linear_model import ElasticNet
from sklearn.ensemble import RandomForestRegressor, StackingRegressor
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.base import clone
from sklearn.linear_model import Ridge

# Optional high-performance libraries
HAS_XGB = HAS_LGB = HAS_CAT = HAS_OPTUNA = False
try:
    from xgboost import XGBRegressor
    HAS_XGB = True
except Exception:
    XGBRegressor = None

try:
    from lightgbm import LGBMRegressor
    HAS_LGB = True
except Exception:
    LGBMRegressor = None

try:
    from catboost import CatBoostRegressor
    HAS_CAT = True
except Exception:
    CatBoostRegressor = None

try:
    import optuna
    HAS_OPTUNA = True
except Exception:
    optuna = None


RANDOM_STATE = 42
N_TRIALS = 50  # reduce/increase depending on compute budget
CV_FOLDS = 5


def load_data(train_path: str = "burn_calories_train.csv", test_path: str = "burn_calories_test.csv"):
    """Load CSV files into pandas DataFrames."""
    train = pd.read_csv(train_path)
    test = pd.read_csv(test_path)
    return train, test


def feature_engineering(df: pd.DataFrame, is_train: bool = True):
    """Create robust features without destroying information.

    - Encode Gender (0 female, 1 male) with safe fallback
    - BMI = weight / (height_m ** 2)
    - hr_duration = Heart_Rate * Duration
    - hr_age = Heart_Rate / (Age + 1)
    - weight_per_min = Weight / (Duration + 1)
    - temp_dev = Body_Temp - 37.0  # deviation from normal
    - nonlinear interactions: squared terms and product interactions
    """
    df = df.copy()

    # Ensure columns are present
    for col in ["Age", "Height", "Weight", "Duration", "Heart_Rate", "Body_Temp", "Gender"]:
        if col not in df.columns:
            df[col] = np.nan

    # User_ID
    if "User_ID" not in df.columns:
        df["User_ID"] = df.index

    # Gender encoding
    def enc_gender(x):
        if pd.isna(x):
            return np.nan
        s = str(x).strip().lower()
        if s in ("m", "male", "man"):
            return 1
        if s in ("f", "female", "woman"):
            return 0
        # attempt to parse
        if s.startswith("m"):
            return 1
        if s.startswith("f"):
            return 0
        return np.nan

    df["Gender_bin"] = df["Gender"].apply(enc_gender)

    # Height in meters
    df["Height_m"] = df["Height"] / 100.0

    # BMI (safe)
    df["BMI"] = df["Weight"] / ((df["Height_m"].replace(0, np.nan)) ** 2)

    # Interaction and ratio features
    df["HRxDuration"] = df["Heart_Rate"] * df["Duration"]
    df["HR_over_Age"] = df["Heart_Rate"] / (df["Age"] + 1)
    df["Weight_per_min"] = df["Weight"] / (df["Duration"] + 1)
    df["Temp_dev"] = df["Body_Temp"] - 37.0

    # Non-linear features
    df["Duration_sq"] = df["Duration"] ** 2
    df["HR_sq"] = df["Heart_Rate"] ** 2
    df["BMI_sq"] = df["BMI"] ** 2

    # Cross-interactions that often help for physiological signals
    df["BMI_x_HR"] = df["BMI"] * df["Heart_Rate"]
    df["Duration_x_TempDev"] = df["Duration"] * df["Temp_dev"]

    # Drop temporary columns we don't want directly
    # Keep Height_m for potential use; otherwise we'll select features later

    return df


def detect_and_remove_leaks(train: pd.DataFrame, features: list, target: str = "Calories"):
    """Detect trivial leaks (features almost perfectly correlated with target)

    - If any feature has absolute Pearson correlation > 0.995 with target,
      it's likely a leak; drop it.
    - Also drop constant columns.
    Returns cleaned feature list and list of dropped columns.
    """
    dropped = []
    clean_features = features.copy()
    for col in features:
        if train[col].nunique() <= 1:
            dropped.append(col)
            clean_features.remove(col)
            continue
        corr = train[[col, target]].dropna().corr().iloc[0, 1]
        if pd.notna(corr) and abs(corr) > 0.995:
            dropped.append(col)
            clean_features.remove(col)
    return clean_features, dropped


def build_preprocessor(train_df: pd.DataFrame, feature_cols: list):
    """Create imputer + scaler pipelines for numeric features.

    We scale features that benefit from it (tree models don't need scaling,
    but ElasticNet does). We'll return a transformer and a scaled version
    of data for linear model use.
    """
    imputer = SimpleImputer(strategy="median")
    scaler = StandardScaler()

    # Fit on training data
    X_num = train_df[feature_cols]
    X_imputed = pd.DataFrame(imputer.fit_transform(X_num), columns=feature_cols)
    X_scaled = pd.DataFrame(scaler.fit_transform(X_imputed), columns=feature_cols)

    preproc = {
        "imputer": imputer,
        "scaler": scaler,
        "feature_cols": feature_cols,
    }
    return preproc, X_imputed, X_scaled


def cv_score(model, X, y, folds=CV_FOLDS):
    """Return RMSE and R2 using KFold CV."""
    kf = KFold(n_splits=folds, shuffle=True, random_state=RANDOM_STATE)
    neg_mse = cross_val_score(model, X, y, scoring="neg_mean_squared_error", cv=kf, n_jobs=-1)
    r2s = cross_val_score(model, X, y, scoring="r2", cv=kf, n_jobs=-1)
    rmses = np.sqrt(-neg_mse)
    return float(rmses.mean()), float(r2s.mean())


def get_oof_predictions(estimator, X, y, X_test=None, folds=CV_FOLDS):
    """Generate out-of-fold predictions for a single estimator.

    Returns:
      oof_train: array of shape (n_samples,)
      test_pred: array of shape (n_test,) averaged over folds (or None if X_test is None)
    """
    kf = KFold(n_splits=folds, shuffle=True, random_state=RANDOM_STATE)
    oof_train = np.zeros(len(y), dtype=float)
    test_pred = np.zeros(X_test.shape[0], dtype=float) if X_test is not None else None

    for fold_idx, (tr_idx, val_idx) in enumerate(kf.split(X)):
        X_tr, X_val = X[tr_idx], X[val_idx]
        y_tr, y_val = y[tr_idx], y[val_idx]
        est = clone(estimator)
        try:
            est.fit(X_tr, y_tr)
            oof_train[val_idx] = est.predict(X_val)
            if X_test is not None:
                test_pred += est.predict(X_test)
        except Exception:
            # If a fold fails, fill with mean of y_tr
            oof_train[val_idx] = y_tr.mean()
            if X_test is not None:
                test_pred += np.full(X_test.shape[0], y_tr.mean())

    if X_test is not None:
        test_pred /= folds

    return oof_train, test_pred


def tune_with_optuna(X, y, model_name: str, n_trials: int = N_TRIALS, cv_folds: int = CV_FOLDS, sample_frac: float = 1.0):
    """Tuning wrapper for different model types using Optuna.

    Accepts `cv_folds` and `sample_frac` to allow faster tuning when desired.
    The function returns a configured estimator (not yet fitted). If Optuna
    or the model library is unavailable, it falls back to sensible defaults.
    """
    if model_name == "xgb" and not HAS_XGB:
        return None
    if model_name == "lgb" and not HAS_LGB:
        return None
    if model_name == "cat" and not HAS_CAT:
        return None

    # If Optuna not available, return sensible defaults
    if not HAS_OPTUNA:
        if model_name == "xgb":
            return XGBRegressor(n_estimators=200, random_state=RANDOM_STATE, verbosity=0)
        if model_name == "lgb":
            return LGBMRegressor(n_estimators=200, random_state=RANDOM_STATE)
        if model_name == "cat":
            return CatBoostRegressor(iterations=200, random_state=RANDOM_STATE, verbose=0)
        if model_name == "rf":
            return RandomForestRegressor(n_estimators=300, random_state=RANDOM_STATE, n_jobs=-1)
        if model_name == "en":
            return ElasticNet(random_state=RANDOM_STATE)

    # Helper: sample dataset for faster trials
    def maybe_sample(Xfull, yfull):
        if sample_frac >= 1.0:
            return Xfull, yfull
        rng = np.random.RandomState(RANDOM_STATE)
        n = max(10, int(len(yfull) * sample_frac))
        idx = rng.choice(len(yfull), n, replace=False)
        return Xfull[idx], yfull[idx]

    def objective_xgb(trial):
        max_estimators = 300 if n_trials < 30 else 1000
        params = {
            "n_estimators": trial.suggest_int("n_estimators", 100, max_estimators, step=50),
            "max_depth": trial.suggest_int("max_depth", 3, 12),
            "learning_rate": trial.suggest_loguniform("learning_rate", 0.01, 0.3),
            "subsample": trial.suggest_float("subsample", 0.5, 1.0),
            "colsample_bytree": trial.suggest_float("colsample_bytree", 0.4, 1.0),
            "random_state": RANDOM_STATE,
            "verbosity": 0,
        }
        model = XGBRegressor(**params)
        Xs, ys = maybe_sample(X, y)
        try:
            rmse_mean, _ = cv_score(model, Xs, ys, folds=cv_folds)
            return rmse_mean
        except Exception as e:
            # If a trial configuration causes training to fail, return a large penalty
            return 1e6

    def objective_lgb(trial):
        max_estimators = 300 if n_trials < 30 else 1000
        params = {
            "n_estimators": trial.suggest_int("n_estimators", 100, max_estimators, step=50),
            "num_leaves": trial.suggest_int("num_leaves", 16, 256),
            "learning_rate": trial.suggest_loguniform("learning_rate", 0.01, 0.3),
            "feature_fraction": trial.suggest_float("feature_fraction", 0.4, 1.0),
            "bagging_fraction": trial.suggest_float("bagging_fraction", 0.4, 1.0),
            "random_state": RANDOM_STATE,
        }
        model = LGBMRegressor(**params)
        Xs, ys = maybe_sample(X, y)
        try:
            rmse_mean, _ = cv_score(model, Xs, ys, folds=cv_folds)
            return rmse_mean
        except Exception:
            return 1e6

    def objective_cat(trial):
        max_iters = 300 if n_trials < 30 else 1000
        params = {
            "iterations": trial.suggest_int("iterations", 100, max_iters, step=50),
            "depth": trial.suggest_int("depth", 4, 10),
            "learning_rate": trial.suggest_loguniform("learning_rate", 0.01, 0.3),
            "random_state": RANDOM_STATE,
            "verbose": 0,
        }
        model = CatBoostRegressor(**params)
        Xs, ys = maybe_sample(X, y)
        try:
            rmse_mean, _ = cv_score(model, Xs, ys, folds=cv_folds)
            return rmse_mean
        except Exception:
            return 1e6

    def objective_rf(trial):
        max_estimators = 300 if n_trials < 30 else 800
        # 'auto' is deprecated/invalid in newer sklearn; use 'sqrt', 'log2' or None
        params = {
            "n_estimators": trial.suggest_int("n_estimators", 100, max_estimators, step=50),
            "max_depth": trial.suggest_int("max_depth", 4, 50),
            "max_features": trial.suggest_categorical("max_features", ["sqrt", "log2", None]),
            "random_state": RANDOM_STATE,
            "n_jobs": -1,
        }
        model = RandomForestRegressor(**params)
        Xs, ys = maybe_sample(X, y)
        try:
            rmse_mean, _ = cv_score(model, Xs, ys, folds=cv_folds)
            return rmse_mean
        except Exception:
            return 1e6

    def objective_en(trial):
        params = {
            "alpha": trial.suggest_loguniform("alpha", 1e-5, 10.0),
            "l1_ratio": trial.suggest_float("l1_ratio", 0.0, 1.0),
            "random_state": RANDOM_STATE,
        }
        model = ElasticNet(**params)
        Xs, ys = maybe_sample(X, y)
        rmse_mean, _ = cv_score(model, Xs, ys, folds=cv_folds)
        return rmse_mean

    study = optuna.create_study(direction="minimize")
    if model_name == "xgb":
        study.optimize(objective_xgb, n_trials=n_trials)
        best = study.best_params
        best_model = XGBRegressor(**best, random_state=RANDOM_STATE, verbosity=0)
    elif model_name == "lgb":
        study.optimize(objective_lgb, n_trials=n_trials)
        best = study.best_params
        best_model = LGBMRegressor(**best, random_state=RANDOM_STATE)
    elif model_name == "cat":
        study.optimize(objective_cat, n_trials=n_trials)
        best = study.best_params
        best_model = CatBoostRegressor(**best, random_state=RANDOM_STATE, verbose=0)
    elif model_name == "rf":
        study.optimize(objective_rf, n_trials=n_trials)
        best = study.best_params
        best_model = RandomForestRegressor(**best, random_state=RANDOM_STATE, n_jobs=-1)
    elif model_name == "en":
        study.optimize(objective_en, n_trials=n_trials)
        best = study.best_params
        best_model = ElasticNet(**best, random_state=RANDOM_STATE)
    else:
        return None

    # Fit the best model on full training later; return the configured estimator
    return best_model


def build_and_evaluate_ensemble(models_dict, X, y):
    """Create a stacking ensemble from provided fitted estimators (or estimators to be fitted).

    - models_dict: dict of name->estimator
    - Returns fitted stacking regressor and CV metrics
    """
    estimators = [(name, clone(est)) for name, est in models_dict.items()]
    final_est = RandomForestRegressor(n_estimators=300, random_state=RANDOM_STATE, n_jobs=-1)
    stack = StackingRegressor(estimators=estimators, final_estimator=final_est, n_jobs=-1, passthrough=False)

    rmse_mean, r2_mean = cv_score(stack, X, y)
    # Fit on full data
    stack.fit(X, y)
    return stack, rmse_mean, r2_mean


def retrain_on_full(estimator, X, y):
    """Retrain estimator on the whole training set."""
    model = clone(estimator)
    model.fit(X, y)
    return model


def save_submission(test_ids, preds, out_path: str = "submission.csv"):
    df = pd.DataFrame({"User_ID": test_ids, "Calories": np.round(preds, 2)})
    df.to_csv(out_path, index=False)
    return df


def main():
    parser = argparse.ArgumentParser(description="Calories prediction pipeline. Use --fast to speed up tuning.")
    parser.add_argument("--fast", action="store_true", help="Use faster tuning (fewer trials, sample data, fewer CV folds)")
    parser.add_argument("--no-log", action="store_true", help="Disable log1p transform in meta-learner (use raw target)")
    parser.add_argument("--force-model", type=str, choices=["xgb", "lgb", "cat", "rf", "en"], help="Force use of a single model (skip stacking).")
    args = parser.parse_args()

    # tune settings
    if args.fast:
        n_trials = max(8, N_TRIALS // 6)
        cv_folds = 3
        sample_frac = 0.4
        print("FAST MODE: n_trials=", n_trials, "cv_folds=", cv_folds, "sample_frac=", sample_frac)
    else:
        n_trials = N_TRIALS
        cv_folds = CV_FOLDS
        sample_frac = 1.0

    # 1) Load
    train, test = load_data()

    # 2) Feature engineering on both
    train_fe = feature_engineering(train, is_train=True)
    test_fe = feature_engineering(test, is_train=False)

    # 3) Define candidate features (exclude raw Gender, Height in cm, etc.)
    candidate_features = [
        "Gender_bin",
        "Age",
        "Height",
        "Weight",
        "Duration",
        "Heart_Rate",
        "Body_Temp",
        "BMI",
        "HRxDuration",
        "HR_over_Age",
        "Weight_per_min",
        "Temp_dev",
        "Duration_sq",
        "HR_sq",
        "BMI_sq",
        "BMI_x_HR",
        "Duration_x_TempDev",
    ]

    # 4) Leak detection
    features, dropped = detect_and_remove_leaks(train_fe, candidate_features, target="Calories")
    if dropped:
        print(f"Dropped potential leak/constant features: {dropped}")

    # 5) Preprocessing: impute and scale
    preproc, X_imputed, X_scaled = build_preprocessor(train_fe, features)

    # Prepared data for tree models (imputed, not scaled)
    X_train_tree = X_imputed.values
    X_test_tree = pd.DataFrame(preproc["imputer"].transform(test_fe[features]), columns=features).values

    # Prepared data for linear model (scaled)
    X_train_lin = X_scaled.values
    X_test_lin = pd.DataFrame(preproc["scaler"].transform(pd.DataFrame(X_test_tree, columns=features)), columns=features).values

    y_train = train_fe["Calories"].values

    # 6) Tuning (use Optuna if available)
    tuned_models = {}

    # Which models to tune: in fast mode we skip some heavy models
    # and use sampling + fewer trials to save time.
    # Tune XGBoost
    if HAS_XGB and not args.fast:
        print("Tuning XGBoost...")
        xgb_model = tune_with_optuna(X_train_tree, y_train, "xgb", n_trials=n_trials, cv_folds=cv_folds, sample_frac=sample_frac)
        if xgb_model is not None:
            tuned_models["xgb"] = xgb_model

    # Tune LightGBM (light and fast)
    if HAS_LGB:
        print("Tuning LightGBM...")
        lgb_model = tune_with_optuna(X_train_tree, y_train, "lgb", n_trials=n_trials, cv_folds=cv_folds, sample_frac=sample_frac)
        if lgb_model is not None:
            tuned_models["lgb"] = lgb_model

    # Tune CatBoost only when not fast and available
    if HAS_CAT and not args.fast:
        print("Tuning CatBoost...")
        cat_model = tune_with_optuna(X_train_tree, y_train, "cat", n_trials=n_trials, cv_folds=cv_folds, sample_frac=sample_frac)
        if cat_model is not None:
            tuned_models["cat"] = cat_model

    # Tune RandomForest (use fewer trials in fast mode)
    print("Tuning RandomForest...")
    rf_trials = max(8, n_trials // 3) if args.fast else max(20, n_trials // 2)
    rf_model = tune_with_optuna(X_train_tree, y_train, "rf", n_trials=rf_trials, cv_folds=cv_folds, sample_frac=sample_frac)
    if rf_model is not None:
        tuned_models["rf"] = rf_model

    # Tune ElasticNet (on scaled data)
    print("Tuning ElasticNet (linear)...")
    en_trials = max(8, n_trials // 3) if args.fast else max(20, n_trials // 2)
    en_model = tune_with_optuna(X_train_lin, y_train, "en", n_trials=en_trials, cv_folds=cv_folds, sample_frac=sample_frac)
    if en_model is not None:
        tuned_models["en"] = en_model

    # 7) Evaluate tuned models using CV and collect best performers
    cv_results = {}
    for name, est in tuned_models.items():
        if name == "en":
            rmse_mean, r2_mean = cv_score(est, X_train_lin, y_train)
        else:
            rmse_mean, r2_mean = cv_score(est, X_train_tree, y_train)
        cv_results[name] = {"rmse": rmse_mean, "r2": r2_mean}

    print("CV results summary:")
    for k, v in cv_results.items():
        print(f" - {k}: RMSE={v['rmse']:.5f}, R2={v['r2']:.5f}")

    # 8) Select top models for stacking (top 3 by RMSE)
    ranked = sorted(cv_results.items(), key=lambda x: x[1]["rmse"]) if cv_results else []
    top_names = [name for name, _ in ranked[:3]]
    print(f"Top models for stacking: {top_names}")

    # Prepare estimators for stacking (fit later)
    estimators_for_stack = {}
    for name in top_names:
        est = tuned_models[name]
        estimators_for_stack[name] = est

    # If fewer than 2 models tuned, fall back to a strong default
    if len(estimators_for_stack) < 2:
        print("Not enough tuned models for stacking; using RandomForest default.")
        estimators_for_stack = {"rf": RandomForestRegressor(n_estimators=300, random_state=RANDOM_STATE, n_jobs=-1)}

    # 9) Create OOF predictions for top models and build a Ridge meta-learner
    print("Generating OOF predictions for stacking/blending...")
    # choose top K models (by CV RMSE) to include
    ranked = sorted(cv_results.items(), key=lambda x: x[1]["rmse"]) if cv_results else []
    top_names = [name for name, _ in ranked[:3]]
    print(f"Top models for blending: {top_names}")

    oof_train_cols = []
    test_pred_cols = []
    for name in top_names:
        est = tuned_models[name]
        # select proper input
        if name == "en":
            X_in = X_train_lin
            X_test_in = X_test_lin
        else:
            X_in = X_train_tree
            X_test_in = X_test_tree

        oof, test_pred = get_oof_predictions(est, X_in, y_train, X_test=X_test_in, folds=cv_folds)
        oof_train_cols.append(oof)
        test_pred_cols.append(test_pred)

    if not oof_train_cols:
        # fallback: use RandomForest default if nothing tuned
        print("No tuned models available for blending; using RandomForest default.")
        rf = RandomForestRegressor(n_estimators=300, random_state=RANDOM_STATE, n_jobs=-1)
        rf = retrain_on_full(rf, X_train_tree, y_train)
        preds = rf.predict(X_test_tree)
        submission_df = save_submission(test_fe["User_ID"].values, preds, out_path="submission.csv")
        print(submission_df.head())
        return

    # Stack OOF columns into matrices
    OOF = np.vstack(oof_train_cols).T
    TEST_P = np.vstack(test_pred_cols).T

    # Optionally use a log-target transform for stability
    use_log = True
    # respect CLI override for log usage
    if args.no_log:
        use_log = False

    if use_log:
        y_for_meta = np.log1p(y_train)
        OOF_meta = np.log1p(OOF)
        OOF_meta = np.nan_to_num(OOF_meta, nan=np.nanmean(OOF_meta), posinf=np.nanmean(OOF_meta), neginf=np.nanmean(OOF_meta))
    else:
        y_for_meta = y_train
        OOF_meta = OOF

    # Train Ridge on OOF predictions
    ridge = Ridge(alpha=1.0, random_state=RANDOM_STATE)
    try:
        ridge.fit(OOF_meta, y_for_meta)
    except Exception:
        # if Ridge fails, fallback to simple average
        print("Ridge failed, falling back to average blending.")
        final_test_preds = TEST_P.mean(axis=1)
    else:
        # build TEST_P for meta (apply log if used)
        if use_log:
            TEST_meta = np.log1p(TEST_P)
            TEST_meta = np.nan_to_num(TEST_meta, nan=np.nanmean(TEST_meta), posinf=np.nanmean(TEST_meta), neginf=np.nanmean(TEST_meta))
        else:
            TEST_meta = TEST_P

        TEST_meta_preds = ridge.predict(TEST_meta)
        if use_log:
            final_test_preds = np.expm1(TEST_meta_preds)
        else:
            final_test_preds = TEST_meta_preds

    # Final safety: replace negatives with small positive
    final_test_preds = np.maximum(final_test_preds, 0.0)

    # If user forced a single model, skip blending and use that model directly
    if args.force_model:
        forced = args.force_model
        if forced in tuned_models:
            chosen = tuned_models[forced]
        else:
            print(f"Forced model {forced} not tuned/available; falling back to RandomForest")
            chosen = RandomForestRegressor(n_estimators=300, random_state=RANDOM_STATE, n_jobs=-1)
        # select input
        Xin = X_train_lin if forced == "en" else X_train_tree
        Xtest_in = X_test_lin if forced == "en" else X_test_tree
        print(f"Training forced model {forced} on full data...")
        final_chosen = retrain_on_full(chosen, Xin, y_train)
        preds_forced = final_chosen.predict(Xtest_in)
        preds_forced = np.maximum(preds_forced, 0.0)
        submission_df = save_submission(test_fe["User_ID"].values, preds_forced, out_path="submission.csv")
        print(submission_df.head())
        return

    # 10) Save submission
    submission_df = save_submission(test_fe["User_ID"].values, final_test_preds, out_path="submission.csv")
    print(submission_df.head())


if __name__ == "__main__":
    main()
