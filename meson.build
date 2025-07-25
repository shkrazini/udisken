project(
	'udisken',
	'cpp',
	version : '0.0.1',
	license : 'GPL-3.0-or-later',
	meson_version : '>=1.1.0',
	default_options : [
		# minimum: c++23
		'cpp_std=c++23',
		'default_library=static',
		'warning_level=3',
	],
)

add_project_arguments(
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
	# Triggers whenever including C headers (e.g. glib).
	# Nowadays it is exceptionally rare to see C-style casts in modern C++.
	# '-Wold-style-cast',
	'-Woverloaded-virtual',
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
	'-DUDISKEN_NAME="' + meson.project_name() + '"',
	'-DUDISKEN_VERSION="' + meson.project_version() + '"',
	language : 'cpp',
)

if get_option('debug')
	add_project_arguments('-D_GLIBCXX_DEBUG=1', language : 'cpp')
endif

glib_dep = dependency('glib-2.0', version : ['>=2.84.0', '<3.0.0'])
sdbus_cpp_dep = dependency('sdbus-c++', version : ['>=2.1.0', '<3.0.0']) # SemVer

# BUG(blackma9ick): werror=false does not work.
# See <https://github.com/mesonbuild/meson/issues/13600>
spdlog_proj = subproject('spdlog', default_options : 'werror=false')
spdlog_dep = spdlog_proj.get_variable('spdlog_dep')

libnotify_dep = dependency('libnotify', version : ['>=0.8.0', '<1.0.0'], required : get_option('notify'))
if libnotify_dep.found()
	add_project_arguments('-DFEATURE_NOTIFY', language : 'cpp')
endif

udisks_sdbus_cpp_proj = subproject(
	'udisks-sdbus-c++',
	default_options : [
		# NOTE: you likely need to change this.
		'udisks_dbus_interface=/home/sofian/source/udisken/dbus/org.freedesktop.UDisks2.xml',
	],
	version : '=2.10-0' # unstable
)
udisks_sdbus_cpp_dep = udisks_sdbus_cpp_proj.get_variable('udisks_sdbus_cpp_dep')

subdir('common')
subdir('udisken')
subdir('udiskend')
