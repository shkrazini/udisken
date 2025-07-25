udisken_sources = [
  'conversions.cpp',
  'main.cpp',
  'drive.cpp',
  'filesystem.cpp',
  'manager.cpp',
]

executable(
  'udisken',
  udisken_sources,
  dependencies: [libnotify_dep, sdbus_cpp_dep, udisks_sdbus_cpp_dep],
  install: true,
)
