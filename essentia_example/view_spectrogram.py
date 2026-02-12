import json
import matplotlib
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

metadata = data["metadata"]

sr = metadata.get("sampleRate", 0)
hop_length = metadata.get("hopSize", 0)
n_mels = metadata.get("numBands", 0)

print(sr, hop_length, n_mels)


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