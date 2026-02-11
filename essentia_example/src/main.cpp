#include <print>
#include <vector>
#include <iostream>
#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>
#include <essentia/pool.h>
#include <portaudio.h>

using namespace std;
using namespace essentia;
using namespace essentia::standard;

int main()
{
    essentia::init();

    const string filename = "audio.wav";

    float targetSR = 44100.0;

    AlgorithmFactory &factory = AlgorithmFactory::instance();

    Algorithm *loader = factory.create("MonoLoader",
                                       "filename", filename,
                                       "sampleRate", targetSR);

    std::vector<float> audio;
    loader->output("audio").set(audio);
    loader->compute();

    const int frameSize = 1024;
    const int hopSize = 512;
    const int numBands = 80;

    Algorithm *frameCutter = factory.create("FrameCutter", "frameSize", frameSize, "hopSize", hopSize);
    Algorithm *windowing = factory.create("Windowing", "type", "hann");
    Algorithm *spectrum = factory.create("Spectrum");
    Algorithm *melBands = factory.create("MelBands",
                                         "numberBands", numBands,
                                         "sampleRate", targetSR,
                                         "lowFrequencyBound", 0,
                                         "highFrequencyBound", targetSR / 2.0f);

    Algorithm *logOp = factory.create("UnaryOperator", "type", "log");

    std::vector<float> frame, windowed, spec, mel, logMel;
    Pool pool;

    frameCutter->input("signal").set(audio);
    frameCutter->output("frame").set(frame);

    windowing->input("frame").set(frame);
    windowing->output("frame").set(windowed);

    spectrum->input("frame").set(windowed);
    spectrum->output("spectrum").set(spec);

    melBands->input("spectrum").set(spec);
    melBands->output("bands").set(mel);

    logOp->input("array").set(mel);
    logOp->output("array").set(logMel);

    while (true)
    {
        frameCutter->compute();
        if (frame.empty())
            break;

        windowing->compute();
        spectrum->compute();
        melBands->compute();
        logOp->compute();

        pool.add("logMel", logMel); // добавляем каждый фрейм
    }

    // Сохраняем в JSON
    Algorithm *yamlOut = factory.create("YamlOutput",
                                        "filename", "logmel_44100.json",
                                        "format", "json"); // важно: "json"

    yamlOut->input("pool").set(pool);
    yamlOut->compute();

    std::cout << "Сохранено: " << "logmel_44100.json"
              << " | SR = " << targetSR
              << " Гц | Фреймов: " << pool.value<std::vector<std::vector<float>>>("logMel").size() << "\n";

    // PortAudio
    Pa_Initialize();
    PaStream *stream = nullptr;

    PaError err = Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, targetSR,
                                       paFramesPerBufferUnspecified, nullptr, nullptr);

    if (err != paNoError)
    {
        println("PA Error: {}", Pa_GetErrorText(err));
        return 1;
    }

    Pa_StartStream(stream);
    println("Playing {} seconds...", static_cast<float>(audio.size()) / targetSR);

    Pa_WriteStream(stream, audio.data(), audio.size());

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    delete loader;
    delete frameCutter;
    delete windowing;
    delete spectrum;
    delete melBands;
    delete logOp;
    delete yamlOut;

    essentia::shutdown();

    return 0;
}