// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <blackma9ick@proton.me>
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Copyright (C) 2025 Sofian-Hedi Krazini
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <https://www.gnu.org/licenses/>.

/// Options and status variables for UDISKEN.

#include "options.hpp"

#include "spdlog/spdlog.h"

#include <cstdlib>
#include <string>

namespace options {

constexpr bool NonZero(std::string_view sv) {
  return !sv.empty() && sv.find_first_not_of("0") != std::string_view::npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

bool NonZeroEnvVar(const std::string& var) {
  auto* const var_value{std::getenv(var.c_str())};
  return var_value != nullptr && NonZero(var_value);
}

bool NotifyEnabled() {
  if (NonZeroEnvVar("UDISKEN_NO_NOTIFY")) {
    spdlog::debug("Notifications disabled by environment.");

    return false;
  }

  return true;
}

}  // namespace options
