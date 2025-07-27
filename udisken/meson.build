executable(
	'udisken',
	'main.cpp',
	dependencies : [
		argparse_dep,
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep,
	],
	link_with : common_lib,
	include_directories : common_include_dir,
	install : true,
)
