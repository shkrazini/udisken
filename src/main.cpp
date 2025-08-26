// UDISKEN: A small Linux automounter.
//
// Copyright (C) 2025 Sofian-Hedi Krazini
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/// Main entrypoint; initiates connection to D-Bus and UDisks.

#include "options.hpp"
#include "udisks.hpp"
#include "utilities.hpp"

#include <argparse/argparse.hpp>
#include <sdbus-c++/IConnection.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <iostream>

auto main(int argc, char* argv[]) -> int {
  argparse::ArgumentParser program{globals::kAppName, globals::kAppVersion};
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

  // Startup message: UDISKEN (version)
  spdlog::info("{} {}", globals::kAppNameUi, globals::kAppVersion);

  if (globals::kDebug || utils::NonZeroEnvironmentVariable("DEBUG") ||
      verbose) {
    spdlog::set_level(spdlog::level::debug);
  }

  const std::unique_ptr<sdbus::IConnection> connection{
      sdbus::createSystemBusConnection()};
  managers::UdisksManager mgr{*connection};
  spdlog::info("Connected to UDisks version {} on D-Bus", mgr.Version());
  managers::UdisksObjectManager obj_mgr{*connection,
                                        options::Options{.notify = !no_notify}};

  spdlog::debug("Entering event loop");
  connection->enterEventLoop();
}
