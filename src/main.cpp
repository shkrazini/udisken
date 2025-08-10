// UDISKEN: Linux drive automounter for the impatient.
// Copyright (C) 2025 BlackMa9ick
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

#include "options.hpp"
#include "udisks.hpp"

#include <argparse/argparse.hpp>
#include <sdbus-c++/IConnection.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#ifdef FEATURE_NOTIFY
#include <libnotify/notify.h>

#include <cstdlib>
#endif  // FEATURE_NOTIFY

#include <iostream>

namespace {

auto InitNotify() -> bool {
#ifdef FEATURE_NOTIFY
  return notify_init(globals::kAppName) &&
         std::atexit([] { notify_uninit(); }) == 0;
#else
  return true;
#endif  // FEATURE_NOTIFY
}

}  // namespace

auto main(int argc, char* argv[]) -> int {
  argparse::ArgumentParser program(globals::kAppName, globals::kAppVer);
  bool no_automount{};
  program.add_argument("--no-automount")
      .help("do not automount")
      .flag()
      .store_into(no_automount);
  bool no_notify{};
  if constexpr (globals::kNotify) {
    program.add_argument("--no-notify")
        .help("do not send desktop notifications")
        .flag()
        .store_into(no_notify);
  }
  bool verbose{};
  program.add_argument("-d", "--debug", "--verbose")
      .help("increase output verbosity")
      .flag()
      .store_into(verbose);
  try {
    program.parse_args(argc, argv);
  } catch (const std::exception& err) {
    spdlog::error("Parsing args failed: {}", err.what());
    std::cerr << program;
    return EXIT_FAILURE;
  }

  if (globals::kDebug || verbose) {
    spdlog::set_level(spdlog::level::debug);
  }

  spdlog::info("UDISKEN - {} - GPLv3", globals::kAppVer);

  if (globals::kNotify && !no_notify && !InitNotify()) {
    spdlog::critical("libnotify initialization failed!");
    return EXIT_FAILURE;
  }
  spdlog::debug("libnotify: {}", globals::kNotify);

  const auto connection = sdbus::createSystemBusConnection();
  managers::UdisksManager mgr{*connection};
  spdlog::info("Connected to UDisks version {} on D-Bus", mgr.Version());
  managers::UdisksObjectManager obj_mgr{
      *connection,
      options::Options{.automount = !no_automount, .notify = !no_notify}};

  spdlog::debug("Entering event loop");
  connection->enterEventLoop();
}
