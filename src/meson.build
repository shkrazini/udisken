udisken_sources = [
	'main.cpp',
	'options.cpp',
	'udisks.cpp',
	'utilities.cpp',
]

executable(
	'udisken',
	udisken_sources,
	dependencies : [
		argparse_dep,
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep
	],
	install : true,
)
