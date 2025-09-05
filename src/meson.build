# SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <blackma9ick@proton.me>
# SPDX-License-Identifier: 0BSD

udisken_sources = [
  'main.cpp',
  'mount.cpp',
  'notify.cpp',
  'options.cpp',
  'udisks.cpp',
]

executable(
  'udisken',
  udisken_sources,
  dependencies : [
    argparse_dep,
    sdbus_cpp_dep,
    spdlog_dep,
    udisks_sdbus_cpp_dep,
  ],
  install : true,
)
