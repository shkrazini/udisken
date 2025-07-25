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

#include "udisks_manager.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/Types.h>

#include <iostream>
#include <print>

// TODO(xlacroixx): make async event loop so we can test properly.
auto main() -> int {
  std::println("udisken {} ", UDISKEN_VERSION);

  const auto connection = sdbus::createSystemBusConnection();

  // XXX(xlacroixx): need to send a message to UDisks in order to wake it up?
  udisken::UdisksManager manager_proxy{*connection};

  std::flush(std::cout);
  connection->enterEventLoop();
}
