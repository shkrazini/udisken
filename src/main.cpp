// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <sofian-hedi.krazini@proton.me>
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

/// Main entrypoint; initiates connection to D-Bus and UDisks.

#include "options.hpp"
#include "udisks.hpp"

#include <argparse/argparse.hpp>
#include <sdbus-c++/IConnection.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
  argparse::ArgumentParser program{globals::kAppName, globals::kAppVersion};
  bool no_log_timestamp{};
  program.add_argument("--no-log-timestamp")
      .help("do not display timestamp when logging")
      .flag()
      .store_into(no_log_timestamp);
  bool no_notify{};
  program.add_argument("--no-notify")
      .help("do not send desktop notifications")
      .flag()
      .store_into(no_notify);
  bool verbose{};
  program.add_argument("-d", "--debug", "--verbose")
      .help("increase output verbosity")
      .flag()
      .store_into(verbose);
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& e) {
    spdlog::critical("{}", e.what());
    std::cerr << program;
    return EXIT_FAILURE;
  }

  if (options::NonZeroEnvVar("UDISKEN_NO_LOG_TIMESTAMP") || no_log_timestamp) {
    // Enable coloring log level, display log level and normal text only.
    spdlog::set_pattern("[%^%l%$] %v");
  }

  if (globals::kDebug || options::NonZeroEnvVar("DEBUG") || verbose) {
    spdlog::set_level(spdlog::level::debug);
  }

  // Startup message: UDISKEN (version)
  spdlog::info("{} {}", globals::kAppNameUi, globals::kAppVersion);

  const auto connection{sdbus::createSystemBusConnection()};
  managers::UdisksManager mgr{*connection};
  spdlog::info("Connected to UDisks version {} on D-Bus", mgr.Version());
  managers::UdisksObjectManager obj_mgr{*connection,
                                        options::Options{.notify = !no_notify}};

  spdlog::debug("Entering event loop");
  connection->enterEventLoop();
}
