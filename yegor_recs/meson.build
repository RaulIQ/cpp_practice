project('yegor_recs', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++26',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)
executable('main', files(
  'src/main.cpp',
  'src/document/document.cpp',
  'src/memory/memory.cpp',
  'src/text/text.cpp'
), include_directories: 'src', install: false)
