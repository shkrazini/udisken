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

/// Utilities, mostly functions depending on enabled features.

#include "utilities.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IProxy.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <cstdlib>
#include <ranges>
#include <string>
#include <vector>

namespace utils {

auto ConvertArrayArrayByte(const std::vector<std::vector<std::uint8_t>>& aay)
    -> std::vector<std::string> {
  return aay | std::views::transform([](const auto& vec) {
           return std::string{std::from_range, vec};
         }) |
         std::ranges::to<std::vector<std::string>>();
}
// TEST(blackma9ick): convert sample data.

constexpr auto NonZero(std::string_view sv) -> bool {
  return !sv.empty() && sv.find_first_not_of("0") != std::string_view::npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

auto NonZeroEnvVar(const std::string& var) -> bool {
  auto* const var_value = std::getenv(var.c_str());
  return var_value != nullptr && NonZero(var_value);
}

}  // namespace utils

namespace notify {

namespace {

void PrintCapabilities(sdbus::IProxy& notify_proxy) {
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

auto CloseNotification(std::uint32_t id) -> bool {
  // TODO(blackma9ick): make (const) global proxy variable, for use in the whole
  // namespace?
  std::unique_ptr<sdbus::IProxy> notify_proxy =
      sdbus::createProxy(kNotifServiceName, kNotifObjectPath);

  spdlog::debug("Closing notification with ID {}", id);

  try {
    notify_proxy->callMethod("CloseNotification")
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

auto Notify(const Notification& notif, ActionInvokedCallback callback) -> bool {
  // TODO(blackma9ick): make (const) global proxy variable, for use in the whole
  // namespace?
  std::unique_ptr<sdbus::IProxy> notify_proxy =
      sdbus::createProxy(kNotifServiceName, kNotifObjectPath);

  PrintCapabilities(*notify_proxy);

  spdlog::debug("Registering notifications actions");
  notify_proxy->uponSignal("ActionInvoked")
      .onInterface(kNotifInterfaceName)
      .call(callback);

  std::uint32_t notif_id{};
  spdlog::debug("Sending notification: [{}] {}", notif.summary, notif.body);
  try {
    // XXX(blackma9ick): if you get
    // "Notifications.Error.ExcessNotificationGeneration" and you have recently
    // upgraded your packages, make sure to reboot ;)
    notify_proxy->callMethod("Notify")
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
