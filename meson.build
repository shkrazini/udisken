project(
  'udisken',
  'cpp',
  version: '0.0.1',
  license: 'GPL-3.0-or-later',
  default_options: [
    # minimum: c++20
    'cpp_std=c++26',
    'warning_level=3',
    'werror=true',
  ],
)

add_project_arguments(
  '-pedantic-errors',
  '-Weffc++',
  '-Wcast-align',
  '-Wcast-qual',
  '-Wconversion',
  '-Wctor-dtor-privacy',
  '-Wdisabled-optimization',
  '-Wformat=2',
  '-Wfloat-equal',
  '-Winvalid-pch',
  '-Wlogical-op',
  '-Wmissing-declarations',
  '-Wmissing-include-dirs',
  '-Wnoexcept',
  '-Wold-style-cast',
  '-Woverloaded-virtual',
  '-Wpadded',
  '-Wredundant-decls',
  '-Wshadow',
  '-Wsign-conversion',
  '-Wsign-promo',
  '-Wstack-protector',
  '-Wstrict-null-sentinel',
  '-Wstrict-overflow=5',
  '-Wswitch-default',
  '-Wundef',
  '-Wuseless-cast',
  '-Wzero-as-null-pointer-constant',
  '-DUDISKEN_VERSION="' + meson.project_version() + '"',
  language: 'cpp',
)

libnotify_dep = dependency('libnotify', version: ['>=0.8.4'])
sdbus_cpp_dep = dependency('sdbus-c++', version: ['>=2.1.0', '<3.0.0']) # SemVer
# NOTE: probably not a hard dependency.
# udisken will still need UDisks to perform any action, but this is clearly not
# a build dependency. The dependency should be enforced in packages by distros.
# udisks = dependency('udisks2', version: ['>=2.10.0', '<3.0.0']) # MAJOR (X.0.0) bumps

udisks_sdbus_cpp_proj = subproject(
  'udisks-sdbus-c++',
  default_options: [
    # NOTE: you may need to change this, depending on your setup.
    'udisks_dbus_interface=/usr/share/dbus-1/interfaces/org.freedesktop.UDisks2.xml',
  ],
  version: ['>=2.10.0', '<3.0.0'] # matches UDisks MAJOR (X.0.0) bumps,,
)
udisks_sdbus_cpp_dep = udisks_sdbus_cpp_proj.get_variable('udisks_sdbus_cpp_dep')

subdir('src')
