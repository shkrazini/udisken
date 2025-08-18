// UDISKEN: Linux drive automounter for the impatient.
// Copyright (C) 2025 Sofian-Hedi Krazini
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

#include "utilities.hpp"

#include "spdlog/spdlog.h"

#include <libnotify/notify.h>

#include <string>
#include <string_view>

namespace options {

auto MountEnabled() -> bool {
  if (utils::NonZeroEnvironmentVariable("UDISKEN_NO_AUTOMOUNT")) {
    spdlog::debug("Automounting disabled by environment.");

    return false;
  }

  return true;
}

auto NotifyEnabled() -> bool {
  if (utils::NonZeroEnvironmentVariable("UDISKEN_NO_NOTIFY")) {
    spdlog::debug("Notifications disabled by environment.");

    return false;
  }

  return globals::kNotify;
}

}  // namespace options
