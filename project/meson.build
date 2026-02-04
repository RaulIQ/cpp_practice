# meson.build
project('mkdir_example', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)

# Источники
sources = files('src/main.cpp')

# Исполняемый файл
executable('main', sources,
  install: false  # не устанавливать системно
)