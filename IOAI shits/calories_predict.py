"""
Train a TabPFN regressor on the calorie-burning dataset and create submission.csv.
Requires: pip install tabpfn torch pandas scikit-learn
"""

from __future__ import annotations

import numpy as np
import pandas as pd
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tabpfn import TabPFNRegressor


TRAIN_PATH = "burn_calories_train.csv"
TEST_PATH = "burn_calories_test.csv"
SUBMISSION_PATH = "submission.csv"

# Feature columns provided by the competition statement.
FEATURE_COLS = [
	"Gender",
	"Age",
	"Height",
	"Weight",
	"Duration",
	"Heart_Rate",
	"Body_Temp",
]
TARGET_COL = "Calories"


def preprocess(df: pd.DataFrame) -> pd.DataFrame:
	df = df.copy()

	# Map gender to numeric to keep the model purely numeric.
	gender_map = {"female": 0, "male": 1}
	df["Gender"] = df["Gender"].str.lower().map(gender_map)

	# Fill any remaining missing values with column medians.
	df = df.fillna(df.median(numeric_only=True))
	return df


def main() -> None:
	train_df = preprocess(pd.read_csv(TRAIN_PATH))
	test_df = preprocess(pd.read_csv(TEST_PATH))

	X = train_df[FEATURE_COLS]
	y = train_df[TARGET_COL]

	scaler = StandardScaler()
	X_scaled = scaler.fit_transform(X)
	X_test_scaled = scaler.transform(test_df[FEATURE_COLS])

	# Quick holdout to gauge RMSE before training on all data.
	X_train, X_val, y_train, y_val = train_test_split(
		X_scaled, y, test_size=0.2, random_state=42
	)

	model = TabPFNRegressor(
		n_estimators=16,
		device="auto",
		ignore_pretraining_limits=True,
	)
	model.fit(X_train, y_train)

	val_pred = model.predict(X_val)
	rmse = np.sqrt(mean_squared_error(y_val, val_pred))
	print(f"Validation RMSE: {rmse:.4f}")

	# Train on full data for the final submission.
	model.fit(X_scaled, y)
	test_pred = model.predict(X_test_scaled)

	submission = pd.DataFrame({"User_ID": test_df["User_ID"], "Calories": test_pred})
	submission.to_csv(SUBMISSION_PATH, index=False)
	print(f"Saved predictions to {SUBMISSION_PATH}")


if __name__ == "__main__":
	main()
