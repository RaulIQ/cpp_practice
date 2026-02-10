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
    const float targetSR = 4000.0f;

    AlgorithmFactory &factory = AlgorithmFactory::instance();

    // === Исправленная часть ===
    Algorithm *mdReader = factory.create("MetadataReader", "filename", filename);

    // Все выходы MetadataReader
    int sampleRate_f = 0.0;
    int channels = 0;
    std::string title, artist, album, comment, genre, tracknumber, date;
    int duration = 0;
    int bitrate = 0;

    Pool tagPool; // ← ОБЯЗАТЕЛЬНО объявляем!

    // Подключаем ВСЁ
    mdReader->output("sampleRate").set(sampleRate_f);
    mdReader->output("channels").set(channels);
    mdReader->output("title").set(title);
    mdReader->output("artist").set(artist);
    mdReader->output("album").set(album);
    mdReader->output("comment").set(comment);
    mdReader->output("genre").set(genre);
    mdReader->output("tracknumber").set(tracknumber);
    mdReader->output("date").set(date);
    mdReader->output("duration").set(duration);
    mdReader->output("bitrate").set(bitrate);
    mdReader->output("tagPool").set(tagPool); // ← вот эта строка была проблемой

    mdReader->compute();

    int originalSR = static_cast<int>(sampleRate_f);

    println("File Info: {} Hz, {} channels", originalSR, channels);

    // MonoLoader на оригинальной частоте
    Algorithm *loader = factory.create("MonoLoader",
                                       "filename", filename,
                                       "sampleRate", originalSR);

    vector<float> rawAudio;
    loader->output("audio").set(rawAudio);
    loader->compute();

    // Ресемплинг
    Algorithm *resampler = factory.create("Resample",
                                          "inputSampleRate", originalSR,
                                          "outputSampleRate", targetSR);

    vector<float> resampledAudio;
    resampler->input("signal").set(rawAudio);
    resampler->output("signal").set(resampledAudio);
    resampler->compute();

    println("Resampling done: {} -> {} samples", rawAudio.size(), resampledAudio.size());

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
    println("Playing {} seconds...", static_cast<float>(resampledAudio.size()) / targetSR);

    Pa_WriteStream(stream, resampledAudio.data(), resampledAudio.size());

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    delete mdReader;
    delete loader;
    delete resampler;
    essentia::shutdown();

    return 0;
}