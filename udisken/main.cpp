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

/// Main entrypoint for commands.

#include "globals.hpp"
#include "managers.hpp"

#include <argparse/argparse.hpp>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <cstdlib>
#include <iostream>
#include <print>

auto main(int argc, char* argv[]) -> int {
  argparse::ArgumentParser program{UDISKEN_NAME, UDISKEN_VERSION};

  program.add_argument("-V", "--verbose")
      .help("increase output verbosity")
      .flag();

  program.add_description("Manipulate disks.");

  // TODO(blackma9ick): support `--` separator

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& e) {
    spdlog::error("{}", e.what());
    std::cerr << program;

    return EXIT_FAILURE;
  }

  if (program["--verbose"] == true || globals::kDebug) {
    spdlog::set_level(spdlog::level::debug);
  }

  const auto connection = sdbus::createSystemBusConnection();

  managers::UdisksManager mgr{*connection};
  spdlog::debug("Connected to UDisks version {} on D-Bus", mgr.Version());

  managers::UdisksObjectManager object_mgr{*connection};
}
