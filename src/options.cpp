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

#include <cstdlib>
#include <string>
#include <string_view>

namespace options {

namespace {

constexpr auto NonZero(std::string_view sv) -> bool {
  // NOLINTNEXTLINE(readability-static-accessed-through-instance)
  return !sv.empty() && sv.find_first_not_of("0") != sv.npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

auto NonZeroEnvironmentVariable(const std::string& var) -> bool {
  const auto var_value = std::getenv(var.c_str());
  return var_value != nullptr && NonZero(var_value);
}

}  // namespace

auto ShouldMount() -> bool {
  if (NonZeroEnvironmentVariable("UDISKEN_NO_AUTOMOUNT")) {
    spdlog::debug("Automounting disabled by environment.");

    return false;
  }

  return true;
}

}  // namespace options
