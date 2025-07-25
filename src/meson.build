udisken_sources = [
  'conversions.cpp',
  'main.cpp',
  'managers.cpp',
  'proxies.cpp',
]

executable(
  'udisken',
  udisken_sources,
  dependencies: [libnotify_dep, sdbus_cpp_dep, udisks_sdbus_cpp_dep],
  install: true,
)
