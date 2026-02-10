#include <print>
#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>

int main()
{
    essentia::init();
    std::println("Essentia version: {}", essentia::version);
    essentia::standard::AlgorithmFactory &factory = essentia::standard::AlgorithmFactory::instance();
    essentia::shutdown();
    return 0;
}