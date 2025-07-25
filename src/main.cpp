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

/// @file
/// @brief Main entrypoint; initiates connection to D-Bus and UDisks.

#include "udisks_manager.hpp"

#include <sdbus-c++/IConnection.h>

#include <iostream>
#include <print>

// TODO(xlacroixx): make async event loop so we can test properly.
auto main() -> int {
  std::println("udisken {} - VERY WIP, WILL BREAK LOL!! ", UDISKEN_VERSION);

  const auto connection = sdbus::createSystemBusConnection();

  udisken::UdisksManager manager_proxy{*connection};

  std::flush(std::cout);
  connection->enterEventLoop();
}
