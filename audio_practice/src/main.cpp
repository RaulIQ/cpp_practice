#include <print>
#include <sndfile.h>
#include <portaudio.h>

#include <vector>

#include <samplerate.h>

std::vector<float> resample(
    const std::vector<float>& input, // <- передаём по ссылке
    int channels,
    int src_rate,
    int dst_rate
) {
    std::vector<float> output;

    SRC_DATA data{};
    data.data_in = input.data();
    data.input_frames = input.size() / channels;
    data.src_ratio = double(dst_rate) / src_rate; // <- double!
    data.end_of_input = 1;

    size_t out_frames = static_cast<size_t>(data.input_frames * data.src_ratio) + 1;
    output.resize(out_frames * channels);
    data.data_out = output.data();
    data.output_frames = out_frames;

    int error = src_simple(&data, SRC_SINC_FASTEST, channels);
    if (error) {
        throw std::runtime_error(std::string("Resampling failed: ") + src_strerror(error));
    }

    output.resize(data.output_frames_gen * channels);
    return output;
}

int main() {
    const char* filename = "assets/audio.wav";

    SF_INFO info{};
    SNDFILE* file = sf_open(filename, SFM_READ, &info);
    if (!file) {
        std::println("Failed to open file");
        return 1;
    }

    std::println("{}", info.samplerate);
    std::println("{}", info.channels);
    std::println("{}", info.frames);

    std::vector<float> samples(info.frames * info.channels);
    sf_readf_float(file, samples.data(), info.frames);
    sf_close(file);

    std::vector<float> resampled = resample(samples, info.channels, info.samplerate, 16000);

    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::println("Pa_Initialize failed");
        return 1;
    }

    PaStream* stream;
    err = Pa_OpenDefaultStream(
        &stream,
        0,                    // no input
        info.channels,        // output channels
        paFloat32,            // float samples
        16000,
        paFramesPerBufferUnspecified,
        nullptr,
        nullptr
    );
    if (err != paNoError) {
        std::println("Pa_OpenDefaultStream failed");
        return 1;
    }

    Pa_StartStream(stream);

    unsigned long frames =
        static_cast<unsigned long>(resampled.size() / info.channels);

    Pa_WriteStream(stream, resampled.data(), frames);

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}