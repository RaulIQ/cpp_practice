import json
import numpy as np
import matplotlib.pyplot as plt
import librosa.display

# 1. Загрузка данных
with open("spectrogram.json", "r") as f:
    data = json.load(f)

# Внимание: в C++ ты сохранил ключ "logMel" (pool.add("logMel", ...))
# а не "spectrogram"
S = np.array(data["logMel"])  

# S имеет форму (Time, MelBands). Для графика нужно (MelBands, Time)
S = S.T 

sr = data.get("metadata.sampleRate", 44100)
hop_length    = data.get("metadata.hopSize", 256)
n_mels = data.get("metadata.numBands", 80)

plt.figure(figsize=(12, 6))

# 2. Отрисовка
# y_axis='mel' заставляет librosa нарисовать правильную логарифмическую линейку частот
# fmax=sr/2 важно, чтобы верхняя граница соответствовала настройкам MelBands в C++
librosa.display.specshow(S, 
                         sr=sr, 
                         hop_length=hop_length, 
                         x_axis='time', 
                         y_axis='mel', 
                         fmax=sr/2,
                         cmap='viridis') # или 'magma', 'inferno'

plt.colorbar(format='%+2.0f dB')
plt.title('Essentia Mel Spectrogram')
plt.tight_layout()
plt.show()