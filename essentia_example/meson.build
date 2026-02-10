# meson.build
project('essentia_example', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)

essentia_dep = dependency('essentia')

# Источники
sources = files('src/main.cpp')

# Исполняемый файл
executable('main', sources,
  dependencies : [essentia_dep],
  install: false 
)