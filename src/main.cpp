// udisken: an automounter that stays quite quiet.
// Copyright (C) 2025 xlacroixx
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

/// Main entrypoint; initiates connection to D-Bus and UDisks.

#include "managers.hpp"

#include <sdbus-c++/IConnection.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <iostream>

// TODO(xlacroixx): make async event loop?
auto main() -> int {
#ifndef NDEBUG
  spdlog::set_level(spdlog::level::debug);
#endif  // !NDEBUG

  spdlog::info("udisken - {}", UDISKEN_VERSION);

  const auto connection = sdbus::createSystemBusConnection();

  managers::UdisksManager manager{*connection};
  spdlog::info("Connected to UDisks2 version {} on D-Bus", manager.Version());

  managers::UdisksObjectManager object_manager{*connection};

  std::flush(std::cout);
  connection->enterEventLoop();
}
