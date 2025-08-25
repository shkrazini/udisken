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
#include <utility>
#include <vector>

namespace utils {

// TEST(blackma9ick): convert sample data.
auto ConvertArrayArrayByte(const std::vector<std::vector<uint8_t>>& aay)
    -> std::vector<std::string> {
  return aay | std::views::transform([](const auto& vec) {
           return std::string{std::from_range, vec};
         }) |
         std::ranges::to<std::vector<std::string>>();
}

constexpr auto NonZero(std::string_view sv) -> bool {
  return !sv.empty() && sv.find_first_not_of("0") != std::string_view::npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

auto NonZeroEnvironmentVariable(const std::string& var) -> bool {
  auto* const var_value = std::getenv(var.c_str());
  return var_value != nullptr && NonZero(var_value);
}

}  // namespace utils

namespace notify {

// TODO(blackma9ick): add open in file manager actions.
auto Notify(const Notification& notif) -> bool {
  sdbus::ServiceName service_name{"org.freedesktop.Notifications"};
  sdbus::ObjectPath object_path{"/org/freedesktop/Notifications"};
  sdbus::InterfaceName interface_name{service_name};
  auto notify_proxy =
      sdbus::createProxy(std::move(service_name), std::move(object_path));

  std::uint32_t notif_id{};
  try {
    // FIXME(blackma9ick): gives an error with "too many notifications sent
    // quickly". For some reason.
    notify_proxy->callMethod("Notify")
        .onInterface(interface_name)
        .withArguments(notif.app_name, notif.replaces_id, notif.app_icon,
                       notif.summary, notif.body, notif.actions, notif.hints,
                       static_cast<std::int32_t>(notif.expire_timeout.count()))
        .storeResultsTo(notif_id);
  } catch (const sdbus::Error& e) {
    spdlog::error("Error after sending notification: {}", e.getMessage());
  }

  // notif_id will always be greater than zero if sending notification
  // succeeded.
  return notif_id != 0;
}

}  // namespace notify
