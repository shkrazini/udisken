libnotify = dependency('libnotify', version : '>=0.8.4')
sdbus_cpp = dependency('sdbus-c++', version : '>=2.1.0')

executable(
  'udisken',
  ['main.cpp', gen_udisks_proxy],
  dependencies : [libnotify, sdbus_cpp],
  install : true,
)
