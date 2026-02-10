#include <print>
#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>
#include <portaudio.h>

using namespace essentia;
using namespace std;

int main()
{
    essentia::init();
    println("Essentia version: {}", essentia::version);
    standard::AlgorithmFactory &factory = standard::AlgorithmFactory::instance();
    standard::Algorithm *loader = factory.create("MonoLoader", "filename", "audio.wav");

    vector<float> audioBuffer;
    loader->output("audio").set(audioBuffer);
    loader->compute();

    cout << "Loaded " << audioBuffer.size() / 44100.0 << " seconds of audio." << endl;

    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        std::println("Pa_Initialize failed");
        return 1;
    }

    PaStream *stream;
    err = Pa_OpenDefaultStream(
        &stream,
        0,         // no input
        1,         // output channels
        paFloat32, // float samples
        44100,
        paFramesPerBufferUnspecified,
        nullptr,
        nullptr);

    if (err != paNoError)
    {
        std::println("Pa_OpenDefaultStream failed");
        return 1;
    }

    Pa_StartStream(stream);

    unsigned long frames =
        static_cast<unsigned long>(audioBuffer.size() / 1);

    Pa_WriteStream(stream, audioBuffer.data(), frames);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    essentia::shutdown();

    return 0;
}