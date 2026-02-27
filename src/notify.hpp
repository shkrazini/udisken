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

#ifndef UDISKEN_NOTIFY_HPP_
#define UDISKEN_NOTIFY_HPP_

#include "options.hpp"

#include <sdbus-c++/IProxy.h>
#include <sdbus-c++/Types.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

/// Send Freedesktop.org desktop notifications.
namespace notify {

/// Callback function type, for when a notification action is invoked.
///
/// Function returns nothing, and take two parameters:
/// - the ID of the notification that the action was invoked on (type:
/// std::uint32_t);
/// - the invoked action key (i.e. name) (type: std::string).
using ActionInvokedCallback = std::function<void(std::uint32_t, std::string)>;

/// Thin struct containing the essential properties of a Freedesktop.org
/// Notification that can be sent to desktop.
///
/// See the Notifications specification for documentation on these properties:
/// <https://specifications.freedesktop.org/notification-spec/latest/>
struct Notification {
  std::string summary;

  std::string body{};
  std::string app_name{globals::kAppName};
  std::string app_icon{};
  std::int32_t expire_timeout{-1};  // -1 means: use expiration time defined by
                                    // the notification server.
  std::uint32_t replaces_id{0};
  std::vector<std::string> actions{};

  std::map<std::string, sdbus::Variant> hints{};
};

static const sdbus::ServiceName kNotifServiceName{
    "org.freedesktop.Notifications"};
static const sdbus::ObjectPath kNotifObjectPath{
    "/org/freedesktop/Notifications"};
static const sdbus::InterfaceName kNotifInterfaceName{kNotifServiceName};

/// Close a Desktop notification with its ID.
///
/// @param notify_proxy Proxy to the org.freedesktop.Notifications service.
/// @param id Desktop notification ID, returned by
/// org.freedesktop.Notifications.Notify.
///
/// @return Notification did not expire or get manually closed, and closed
/// successfully.
bool CloseNotification(sdbus::IProxy& notify_proxy, std::uint32_t id);

/// Send a desktop notification.
///
/// @param notify_proxy Proxy to the org.freedesktop.Notifications service.
/// @param notification Notification.
///
/// @return Successfully sent the notification.
bool Notify(sdbus::IProxy& notify_proxy, const Notification& notif,
            ActionInvokedCallback callback);

}  // namespace notify

#endif  // UDISKEN_NOTIFY_HPP_
