import json
import numpy as np
import matplotlib.pyplot as plt

with open("spectrogram.json") as f:
    data = json.load(f)

S = np.array(data["spectrogram"])  # shape (num_frames, num_bins)

# Получаем метаданные
frameSize = 4096
hopSize   = 256
sampleRate = 44100

num_frames, num_bins = S.shape

# Оси в реальных единицах
times = np.arange(num_frames) * hopSize / sampleRate  # в секундах
freqs = np.arange(num_bins) * sampleRate / frameSize  # в Гц

plt.figure(figsize=(12, 6))
plt.imshow(S.T, origin="lower", aspect="auto",
           extent=[times[0], times[-1], freqs[0], freqs[-1]])
plt.xlabel("Time (s)")
plt.ylabel("Frequency (Hz)")
plt.colorbar(label="Amplitude (dB)")
plt.show()
