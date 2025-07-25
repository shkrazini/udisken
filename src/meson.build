udisken_sources = [
	'conversions.cpp',
	'main.cpp',
	'managers.cpp',
	'objects.cpp',
	'interfaces.cpp',
	'utilities.cpp',
]

executable(
	'udisken',
	udisken_sources,
	dependencies : [
		glib_dep,
		libnotify_dep,
		sdbus_cpp_dep,
		spdlog_dep,
		udisks_sdbus_cpp_dep
	],
	install : true,
)
