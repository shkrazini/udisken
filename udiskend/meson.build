executable(
	'udiskend',
	'main.cpp',
	dependencies : [
		glib_dep,
		libnotify_dep,
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep,
	],
	link_with : common_lib,
	include_directories : common_include_dir,
	install : true,
)
