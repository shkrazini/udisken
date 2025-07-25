libnotify = dependency('libnotify', version : ['>=0.8.4'           ])
sdbus_cpp = dependency('sdbus-c++', version : ['>=2.1.0',  '<3.0.0']) # semver
udisks    = dependency('udisks2',   version : ['>=2.10.0', '<3.0.0']) # X.0.0 (major) bumps
udisks_sdbus_cpp_dep = dependency(
  'udisks-sdbus-cpp',
  fallback: ['udisks-sdbus-cpp', 'udisks_sdbus_cpp_dep'],
  default_options: [
    # NOTE: you may need to change this, depending on your setup.
    'dbus_interface_path=/usr/share/dbus-1/interfaces/org.freedesktop.UDisks2.xml',
  ],
)

executable(
  'udisken',
  ['main.cpp'],
  dependencies: [libnotify, sdbus_cpp, udisks, udisks_sdbus_cpp_dep],
  install: true,
)
