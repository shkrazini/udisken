libnotify = dependency('libnotify', version : ['>=0.8.4'           ])
sdbus_cpp = dependency('sdbus-c++', version : ['>=2.1.0',  '<3.0.0']) # semver
udisks    = dependency('udisks2',   version : ['>=2.10.0', '<3.0.0']) # X.0.0 (major) bumps

executable(
  'udisken',
  ['main.cpp', gen_udisks_proxy],
  dependencies : [libnotify, sdbus_cpp, udisks],
  install : true,
)
