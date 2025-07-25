executable(
	'udisken',
	'main.cpp',
	dependencies : [
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep,
	],
	link_with : common_lib,
	include_directories : common_include_dir,
	install : true,
)

install_symlink(
	'udiskie-info',
	pointing_to : 'udisken',
	install_dir : get_option('bindir'),
)

install_symlink(
	'udiskie-mount',
	pointing_to : 'udisken',
	install_dir : get_option('bindir'),
)

install_symlink(
	'udiskie-unmount',
	pointing_to : 'udisken',
	install_dir : get_option('bindir'),
)
