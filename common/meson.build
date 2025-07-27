common_sources = [
	'interfaces.cpp',
	'managers.cpp',
	'objects.cpp',
	'utilities.cpp',
]

common_include_dir = include_directories('.')

common_lib = library(
	'udisken-common',
	common_sources,
	dependencies : [
		libnotify_dep,
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep,
	],
)
