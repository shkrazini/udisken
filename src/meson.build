libnotify = dependency('libnotify', version : '>=0.8.4' )
sdbus_cpp = dependency('sdbus-c++', version : '>=2.1.0' )
udisks    = dependency('udisks2',   version : '>=2.10.0')

executable(
  'udisken',
  ['main.cpp', gen_udisks_proxy],
  dependencies : [libnotify, sdbus_cpp, udisks],
  install : true,
)
