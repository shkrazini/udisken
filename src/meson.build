udisken_sources = [
  'conversions.cpp',
  'main.cpp',
  'udisks_drive.cpp',
  'udisks_filesystem.cpp',
  'udisks_manager.cpp',
]

executable(
  'udisken',
  udisken_sources,
  dependencies: [libnotify_dep, sdbus_cpp_dep, udisks_sdbus_cpp_dep],
  install: true,
)
