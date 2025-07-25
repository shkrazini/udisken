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

/// Utilities, mostly functions depending on enabled features.

#include "utilities.hpp"

#ifdef FEATURE_NOTIFY
#include "spdlog/spdlog.h"

#include <glib.h>
#include <libnotify/notification.h>

#include <memory>
#endif  // FEATURE_NOTIFY

namespace utils {

auto Notify([[maybe_unused]] const Notification& notification) -> bool {
#ifdef FEATURE_NOTIFY
  // notify_notification_new gives us ownership of the notification.
  auto notif = std::make_unique<NotifyNotification>(*notify_notification_new(
      notification.summary.c_str(), notification.body.c_str(),
      notification.icon.c_str()));

  auto shown = notify_notification_show(notif.get(), nullptr) == FALSE;
  if (!shown) {
    spdlog::warn("Failed to show notification");
  }

  return shown;
#else
  return false;
#endif  // FEATURE_NOTIFY
}

}  // namespace utils
