project(
  'udisken',
  'cpp',
  version: '0.0.1',
  license: 'GPL-3.0-or-later',
  default_options : [
    # minimum: c++20
    'cpp_std=c++26',
    'warning_level=3',
    'werror=true',
  ],
)
add_project_arguments('-pedantic-errors', '-Weffc++', '-Wcast-align',
  '-Wcast-qual', '-Wconversion', '-Wctor-dtor-privacy',
  '-Wdisabled-optimization', '-Wformat=2', '-Wfloat-equal', '-Winvalid-pch',
  '-Wlogical-op', '-Wmissing-declarations',
  '-Wmissing-include-dirs', '-Wnoexcept', '-Wold-style-cast',
  '-Woverloaded-virtual', '-Wpadded', '-Wredundant-decls', '-Wshadow',
  '-Wsign-conversion', '-Wsign-promo', '-Wstack-protector',
  '-Wstrict-null-sentinel', '-Wstrict-overflow=5', '-Wswitch-default',
  '-Wundef', '-Wuseless-cast', '-Wzero-as-null-pointer-constant',
  language : 'cpp')

xml2cpp_prog = find_program('sdbus-c++-xml2cpp')

gen_udisks_adaptor = custom_target(
  'udisks-adaptor',
  output : ['udisks-adaptor.hpp'],
  input : ['dbus/org.freedesktop.UDisks2.xml'],
  command : [xml2cpp_prog, '@INPUT@', '--adaptor=@OUTPUT0@'],
)

gen_udisks_proxy = custom_target(
  'udisks-proxy',
  output : ['udisks-proxy.hpp'],
  input : ['dbus/org.freedesktop.UDisks2.xml'],
  command : [xml2cpp_prog, '@INPUT@', '--proxy=@OUTPUT0@'],
)

subdir('src')
