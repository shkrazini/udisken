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

/// Send Freedesktop.org desktop notifications from here.

#include "notify.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IProxy.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <string>
#include <vector>

namespace notify {

namespace {

/// Print the notification server's capabilities as verbose output.
/// See Freedesktop.org Notifications documentation:
/// <https://specifications.freedesktop.org/notification-spec/latest/>
void DebugCapabilities(sdbus::IProxy& notify_proxy) {
  std::vector<std::string> caps{};
  notify_proxy.callMethod("GetCapabilities")
      .onInterface(kNotifInterfaceName)
      .storeResultsTo(caps);

  spdlog::debug("== Notification server capabilities ==");
  for (const auto& cap : caps) {
    spdlog::debug("{}", cap);
  }
  spdlog::debug("== End of capabilities ==");
}

}  // namespace

bool CloseNotification(sdbus::IProxy& notify_proxy, std::uint32_t id) {
  spdlog::debug("Closing notification with ID {}", id);

  try {
    notify_proxy.callMethod("CloseNotification")
        .onInterface(kNotifInterfaceName)
        .withArguments(id);
  } catch (const sdbus::Error& e) {
    // If the notification expired or already got closed, the D-Bus error
    // message will be empty.
    if (!e.getMessage().empty()) {
      spdlog::error("Error when trying to close notification: {}", e.what());

      return false;
    }
  }

  return true;
}

bool Notify(sdbus::IProxy& notify_proxy, const Notification& notif,
            ActionInvokedCallback callback) {
  DebugCapabilities(notify_proxy);

  spdlog::debug("Registering notifications actions");
  notify_proxy.uponSignal("ActionInvoked")
      .onInterface(kNotifInterfaceName)
      .call(callback);

  std::uint32_t notif_id{};
  spdlog::debug("Sending notification: [{}] {}", notif.summary, notif.body);
  try {
    // XXX: if you get "Notifications.Error.ExcessNotificationGeneration" and
    // you have recently upgraded your packages, make sure to reboot ;)
    notify_proxy.callMethod("Notify")
        .onInterface(kNotifInterfaceName)
        .withArguments(notif.app_name, notif.replaces_id, notif.app_icon,
                       notif.summary, notif.body, notif.actions, notif.hints,
                       notif.expire_timeout)
        .storeResultsTo(notif_id);
  } catch (const sdbus::Error& e) {
    spdlog::error("Error after sending notification: {}", e.what());
  }

  // notif_id will always be greater than zero if sending notification
  // succeeded.
  return notif_id != 0;
}

}  // namespace notify
