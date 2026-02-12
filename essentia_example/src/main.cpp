#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>
#include <essentia/pool.h>
#include <vector>
#include <string>

using namespace std;
using namespace essentia;
using namespace essentia::standard;

int main()
{
    essentia::init();

    string filename = "audio.wav";
    float sampleRate = 44100.0;

    int frameSize = 2048;
    int hopSize = 256;
    const int numBands = 128;

    AlgorithmFactory &factory = AlgorithmFactory::instance();

    // ===== 1. Загрузка аудио =====
    Algorithm *loader = factory.create("MonoLoader",
                                       "filename", filename,
                                       "sampleRate", sampleRate);

    vector<Real> audio;
    loader->output("audio").set(audio);
    loader->compute();

    // ===== 2. Алгоритмы для STFT =====
    Algorithm *frameCutter = factory.create("FrameCutter",
                                            "frameSize", frameSize,
                                            "hopSize", hopSize);

    Algorithm *windowing = factory.create("Windowing",
                                          "type", "hann");

    Algorithm *spectrum = factory.create("PowerSpectrum",
                                         "size", frameSize);

    Algorithm *melBands = factory.create("MelBands",
                                         "numberBands", numBands,
                                         "sampleRate", sampleRate,
                                         "lowFrequencyBound", 0,
                                         "highFrequencyBound", sampleRate / 2.0f);

    Algorithm *lin2db = factory.create("UnaryOperator",
                                       "type", "lin2db");

    // ===== 3. Подключение =====
    frameCutter->input("signal").set(audio);

    vector<Real> frame, windowed, spec, mel, logMel, melSafe, melLog;

    frameCutter->output("frame").set(frame);

    Pool pool; // сюда будем складывать спектрограмму

    // ===== 4. Цикл по кадрам =====
    while (true)
    {

        frameCutter->compute();
        if (frame.empty())
            break;

        windowing->input("frame").set(frame);
        windowing->output("frame").set(windowed);
        windowing->compute();

        spectrum->input("signal").set(windowed);
        spectrum->output("powerSpectrum").set(spec);
        spectrum->compute();

        melBands->input("spectrum").set(spec);
        melBands->output("bands").set(mel);
        melBands->compute();

        // mel уже получен из MelBands

        lin2db->input("array").set(mel);
        lin2db->output("array").set(logMel);
        lin2db->compute();

        // добавляем каждый кадр спектра
        pool.add("logMel", logMel);
    }

    // ===== 5. Сохранение =====
    pool.set("metadata.sampleRate", sampleRate);
    pool.set("metadata.frameSize", frameSize);
    pool.set("metadata.hopSize", hopSize);
    pool.set("metadata.numBands", numBands);

    Algorithm *out = factory.create("YamlOutput",
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
