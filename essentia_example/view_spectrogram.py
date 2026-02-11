import json
import numpy as np
import matplotlib.pyplot as plt

with open("logmel_44100.json") as f:
    data = json.load(f)

logmel = np.array(data['logMel']).T

sr = 44100
hop_size = 512

n_frames = logmel.shape[1]
times = np.arange(n_frames) * hop_size / sr

plt.figure(figsize=(12,6))
plt.imshow(
    logmel,
    aspect='auto',
    origin='lower',
    cmap='magma',
    extent=[times[0], times[-1], 0, logmel.shape[0]]
)

plt.xlabel("Time (seconds)")
plt.ylabel("Mel bands")
plt.colorbar()
plt.tight_layout()
plt.savefig("spectrogram_seconds.png", dpi=300)
