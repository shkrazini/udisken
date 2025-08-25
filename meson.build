project(
  'udisken',
  'cpp',
  version : '0.0.1',
  license : 'Apache-2.0',
  meson_version : '>=1.4.0',
  default_options : [
    # minimum: c++23
    'cpp_std=c++23',
	'b_ndebug=if-release',
    'warning_level=3',
    'werror=true',
  ],
)

add_project_arguments(
  '-DUDISKEN_NAME="' + meson.project_name() + '"',
  '-DUDISKEN_NAME_UI="' + meson.project_name().to_upper() + '"',
  '-DUDISKEN_VERSION="' + meson.project_version() + '"',
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
  '-Wnon-virtual-dtor',
  '-Wold-style-cast',
  '-Woverloaded-virtual',
  '-Wredundant-decls',
  '-Wshadow',
  '-Wsign-conversion',
  '-Wsign-promo',
  '-Wstack-protector',
  '-Wstrict-null-sentinel',
  '-Wstrict-overflow',
  '-Wswitch-default',
  '-Wundef',
  '-Wuseless-cast',
  '-Wzero-as-null-pointer-constant',
  language : 'cpp',
)

if get_option('debug')
  add_project_arguments('-D_GLIBCXX_DEBUG=1', language : 'cpp')
endif

# Setting include_type to system, is a workaround for silencing warnings coming
# from libraries. See <https://github.com/mesonbuild/meson/issues/13600>
argparse_dep = dependency(
  'argparse',
  version : ['>=3.1', '<4.0'],
  include_type : 'system',
  fallback : ['argparse', 'argparse_dep'],
)

sdbus_cpp_dep = dependency(
  'sdbus-c++',
  version : ['>=2.1.0', '<3.0.0'], # Follows Semantic Versioning.
  static : false, # LGPL
)

spdlog_dep = dependency(
  'spdlog',
  version : ['>=1.15.0', '<2.0.0'],
  fallback : ['spdlog', 'spdlog_dep'],
)

dbus_interface = files('dbus/org.freedesktop.UDisks2.xml')
udisks_sdbus_cpp_proj = subproject(
  'udisks-sdbus-c++',
    default_options : [
      'udisks_dbus_interface=' + dbus_interface[0].full_path(),
    ],
    version : '=2.10-1'
)
udisks_sdbus_cpp_dep = udisks_sdbus_cpp_proj.get_variable('udisks_sdbus_cpp_dep')

install_data('udisken.service', install_dir : '/usr/lib/systemd/user')

subdir('src')
