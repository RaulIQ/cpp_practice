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
    float targetSR = 44100.0; // Возьмем стандартную частоту для спектра
    int frameSize = 4096;

    AlgorithmFactory& factory = AlgorithmFactory::instance();

    // 1. Загрузка аудио
    Algorithm* loader = factory.create("MonoLoader", "filename", filename, "sampleRate", targetSR);
    vector<float> audio;
    loader->output("audio").set(audio);
    loader->compute();

    // 2. Берем только первые 4096 сэмплов (как в вашем примере dft_input = array[:4096])
    vector<float> frame;
    frame.assign(audio.begin(), audio.begin() + frameSize);

    // ===== 1. Окно Хэннинга =====
    Algorithm* win = factory.create("Windowing", 
                                    "type", "hann",
                                    "size", frameSize);

    vector<float> windowedFrame;
    win->input("frame").set(frame);
    win->output("frame").set(windowedFrame);
    win->compute();

    // ===== 2. FFT =====

    Algorithm* spec = factory.create("Spectrum", "size", frameSize);

    vector<float> spectrum;

    spec->input("frame").set(windowedFrame);
    spec->output("spectrum").set(spectrum);

    spec->compute();

    // 4. Сохраняем в Pool
    Pool pool;
    pool.set("spectrum", spectrum);
    pool.set("metadata.sampleRate", targetSR);
    pool.set("metadata.frameSize", frameSize);

    Algorithm* yamlOut = factory.create("YamlOutput", 
                                        "filename", "spectrum.json", 
                                        "format", "json");
    yamlOut->input("pool").set(pool);
    yamlOut->compute();

    // Очистка
    delete loader; delete win; delete spec; delete yamlOut;
    essentia::shutdown();

    return 0;
}