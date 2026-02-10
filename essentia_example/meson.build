# meson.build
project('essentia_example', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)

essentia_dep = dependency('essentia', required: true)
dependency_portaudio = dependency('portaudio-2.0', required: true)

# Источники
sources = files('src/main.cpp')

# Исполняемый файл
executable('main', sources,
  dependencies : [essentia_dep, dependency_portaudio],
  install: false 
)