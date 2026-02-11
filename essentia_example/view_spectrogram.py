import json
import numpy as np
import matplotlib.pyplot as plt

# 1. Загрузка данных
with open('spectrum.json') as f:
    data = json.load(f)

# Essentia Spectrum возвращает N/2 + 1 элементов
amplitude = np.array(data['spectrum'])
sr = data['metadata']['sampleRate']
print(sr)
n_fft = data['metadata']['frameSize']

# 2. Перевод в децибелы (аналог librosa.amplitude_to_db)
# Избегаем логарифма нуля, добавляя маленькое число (1e-10)
amplitude_db = 20 * np.log10(amplitude / np.max(amplitude) + 1e-10)

# 3. Расчет частотной сетки (аналог librosa.fft_frequencies)
frequencies = np.linspace(0, sr / 2, len(amplitude))

# 4. Отрисовка
plt.figure(figsize=(12, 5))
plt.plot(frequencies, amplitude_db)
plt.xscale('log') # Логарифмическая шкала как в вашем примере
plt.grid(True, which="both", ls="-", alpha=0.5)

plt.title("Amplitude Spectrum (from Essentia C++)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude (dB)")
plt.xlim(20, sr / 2) # Ограничим слышимым диапазоном
plt.ylim(-80, 5)     # Типичный диапазон для дБ
plt.show()