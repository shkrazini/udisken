udisken_sources = [
  'conversions.cpp',
  'main.cpp',
  'managers.cpp',
  'proxies.cpp',
]

executable(
  'udisken',
  udisken_sources,
  dependencies: [sdbus_cpp_dep, spdlog_dep, udisks_sdbus_cpp_dep],
  install: true,
)
