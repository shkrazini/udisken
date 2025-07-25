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

/// Main entrypoint; initiates connection to D-Bus and UDisks.

#include "globals.hpp"
#include "managers.hpp"

#include <sdbus-c++/IConnection.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#ifdef FEATURE_NOTIFY
#include <glib.h>
#include <libnotify/notify.h>

#include <cstdlib>
#endif  // FEATURE_NOTIFY

// TODO(blackma9ick): make async event loop?
auto main() -> int {
  if constexpr (globals::kDebug) {
    spdlog::set_level(spdlog::level::debug);
  }

  spdlog::info("UDISKEN - {} - GPLv3", UDISKEN_VERSION);

#ifdef FEATURE_NOTIFY
  if (notify_init(globals::kAppName) == FALSE) {
    spdlog::critical("libnotify initialization failed!");

    return EXIT_FAILURE;
  }

  if (std::atexit([] noexcept { notify_uninit(); }) != 0) {
    spdlog::critical("libnotify uninitialization registration failed!");

    return EXIT_FAILURE;
  }
#endif  // FEATURE_NOTIFY
  spdlog::info("libnotify: {}", globals::kNotify);

  const auto connection = sdbus::createSystemBusConnection();

  managers::UdisksManager mgr{*connection};
  spdlog::info("Connected to UDisks version {} on D-Bus", mgr.Version());

  managers::UdisksObjectManager object_mgr{*connection};

  spdlog::debug("Entering event loop");
  connection->enterEventLoop();
}
