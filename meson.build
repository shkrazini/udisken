project(
  'udisken',
  'cpp',
  version: '0.0.1',
  license: 'GPL-3.0-or-later',
  default_options : [
    # minimum: c++20
    'cpp_std=c++26',
    'warning_level=3',
    'werror=true',
  ],
)
add_project_arguments('-pedantic-errors', '-Weffc++', '-Wcast-align',
  '-Wcast-qual', '-Wconversion', '-Wctor-dtor-privacy',
  '-Wdisabled-optimization', '-Wformat=2', '-Wfloat-equal', '-Winvalid-pch',
  '-Wlogical-op', '-Wmissing-declarations',
  '-Wmissing-include-dirs', '-Wnoexcept', '-Wold-style-cast',
  '-Woverloaded-virtual', '-Wpadded', '-Wredundant-decls', '-Wshadow',
  '-Wsign-conversion', '-Wsign-promo', '-Wstack-protector',
  '-Wstrict-null-sentinel', '-Wstrict-overflow=5', '-Wswitch-default',
  '-Wundef', '-Wuseless-cast', '-Wzero-as-null-pointer-constant',
  language : 'cpp')

)

subdir('src')
