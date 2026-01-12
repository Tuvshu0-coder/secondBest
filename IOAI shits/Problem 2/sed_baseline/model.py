from __future__ import annotations

import torch
from torch import nn


class CRNN(nn.Module):
    def __init__(
        self,
        n_mels: int,
        n_classes: int,
        gru_hidden: int = 128,
        conv_channels: int = 128,
        dropout: float = 0.2,
    ) -> None:
        super().__init__()

        self.conv = nn.Sequential(
            nn.Conv2d(1, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=(2, 1)),
            nn.Dropout(dropout),

            nn.Conv2d(32, 64, kernel_size=3, padding=1),
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=(2, 1)),
            nn.Dropout(dropout),

            nn.Conv2d(64, conv_channels, kernel_size=3, padding=1),
            nn.BatchNorm2d(conv_channels),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(kernel_size=(2, 1)),
            nn.Dropout(dropout),
        )

        # After 3x (2,1) pooling on freq: n_mels / 8
        freq_after = n_mels // 8
        if freq_after < 1:
            raise ValueError("n_mels too small for pooling stack")

        self.gru = nn.GRU(
            input_size=conv_channels * freq_after,
            hidden_size=gru_hidden,
            num_layers=1,
            batch_first=True,
            bidirectional=True,
        )
        self.head = nn.Sequential(
            nn.Dropout(dropout),
            nn.Linear(gru_hidden * 2, n_classes),
        )

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        # x: (B, 1, n_mels, T)
        z = self.conv(x)  # (B, C, F, T)
        z = z.permute(0, 3, 1, 2).contiguous()  # (B, T, C, F)
        z = z.view(z.size(0), z.size(1), -1)  # (B, T, C*F)
        z, _ = self.gru(z)  # (B, T, 2H)
        logits = self.head(z)  # (B, T, n_classes)
        return logits
