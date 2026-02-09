project('audio_practice', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)

cc = meson.get_compiler('cpp')

inc = include_directories('include')

dependency_sndfile = dependency('sndfile', required: true)
dependency_portaudio = dependency('portaudio-2.0', required: true)

samplerate_lib = cc.find_library('samplerate', required: true)

sources = files(
  'src/main.cpp',
)

executable('main',
  sources,
  include_directories: inc,
  dependencies: [
    dependency_sndfile,
    dependency_portaudio,
    samplerate_lib,
  ],
  install: false
)
