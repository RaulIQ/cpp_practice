project('pingpong', 'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++23',
    'buildtype=debugoptimized',
    'warning_level=3'
  ]
)

inc = include_directories('include')

sources = files(
  'src/main.cpp',
  'src/thread_client_session.cpp',
  'src/ping_pong_handler.cpp'
)

executable('main',
  sources,
  include_directories: inc,
  install: false
)
