#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>
#include <essentia/pool.h>
#include <vector>
#include <string>

using namespace std;
using namespace essentia;
using namespace essentia::standard;

int main() {
    essentia::init();

    string filename = "audio.wav";
    float sampleRate = 44100.0;

    int frameSize = 4096;   // как в librosa по умолчанию
    int hopSize   = 256;    // как в librosa по умолчанию

    AlgorithmFactory& factory = AlgorithmFactory::instance();

    // ===== 1. Загрузка аудио =====
    Algorithm* loader = factory.create("MonoLoader",
                                       "filename", filename,
                                       "sampleRate", sampleRate);

    vector<Real> audio;
    loader->output("audio").set(audio);
    loader->compute();


    // ===== 2. Алгоритмы для STFT =====
    Algorithm* frameCutter = factory.create("FrameCutter",
                                            "frameSize", frameSize,
                                            "hopSize", hopSize);

    Algorithm* windowing = factory.create("Windowing",
                                          "type", "hann");

    Algorithm* spectrum = factory.create("Spectrum",
                                         "size", frameSize);

    // ===== 3. Подключение =====
    frameCutter->input("signal").set(audio);

    vector<Real> frame, windowed, spec, specDB;

    frameCutter->output("frame").set(frame);


    Pool pool;  // сюда будем складывать спектрограмму


    // ===== 4. Цикл по кадрам =====
    while (true) {

        frameCutter->compute();
        if (frame.empty()) break;

        windowing->input("frame").set(frame);
        windowing->output("frame").set(windowed);
        windowing->compute();

        spectrum->input("frame").set(windowed);
        spectrum->output("spectrum").set(spec);
        spectrum->compute();

        vector<Real> specDB(spec.size());

        Real maxValue = 1e-10;  // защита от log(0)

        // ищем максимум
        for (size_t i = 0; i < spec.size(); ++i) {
            if (spec[i] > maxValue) maxValue = spec[i];
        }

        // перевод в dB
        for (size_t i = 0; i < spec.size(); ++i) {
            specDB[i] = 20.0 * log10(spec[i] / maxValue + 1e-10);
        }

        // добавляем каждый кадр спектра
        pool.add("spectrogram", specDB);
    }


    // ===== 5. Сохранение =====
    pool.set("metadata.sampleRate", sampleRate);
    pool.set("metadata.frameSize", frameSize);
    pool.set("metadata.hopSize", hopSize);

    Algorithm* out = factory.create("YamlOutput",
                                    "filename", "spectrogram.json",
                                    "format", "json");

    out->input("pool").set(pool);
    out->compute();


    // ===== очистка =====
    delete loader;
    delete frameCutter;
    delete windowing;
    delete spectrum;
    delete out;

    essentia::shutdown();
    return 0;
}
