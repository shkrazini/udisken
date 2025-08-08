// UDISKEN: Linux drive automounter for the impatient.
// Copyright (C) BlackMa9ick
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <https://www.gnu.org/licenses/>.

/// Options and status variables for UDISKEN.

#include "options.hpp"

#include "spdlog/spdlog.h"

#include <libnotify/notify.h>

#include <cstdlib>
#include <string>
#include <string_view>

namespace options {

namespace {

/// Checks if the string view has a non-zero value.
///
/// @param sv Typical string view.
/// @return True if string view is non-empty (regardless of the original
/// string's content), and its content contains characters other than '0'.
constexpr auto NonZero(std::string_view sv) -> bool {
  // NOLINTNEXTLINE(readability-static-accessed-through-instance)
  return !sv.empty() && sv.find_first_not_of("0") != sv.npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

/// Checks if the environment variable is defined and has a non-zero value.
///
/// @param var Name of the environment variable.
/// @return True if the environment variable is defined and is non-zero.
auto NonZeroEnvironmentVariable(const std::string& var) -> bool {
  const auto var_value = std::getenv(var.c_str());
  return var_value != nullptr && NonZero(var_value);
}

}  // namespace

auto MountEnabled() -> bool {
  if (NonZeroEnvironmentVariable("UDISKEN_NO_AUTOMOUNT")) {
    spdlog::debug("Automounting disabled by environment.");

    return false;
  }

  return true;
}

auto NotifyEnabled() -> bool {
  if (NonZeroEnvironmentVariable("UDISKEN_NO_NOTIFY")) {
    spdlog::debug("Notifications disabled by environment.");

    return false;
  }

  return globals::kNotify;
}

}  // namespace options
