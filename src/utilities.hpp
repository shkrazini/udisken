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

#ifndef UDISKEN_UTILITIES_HPP_
#define UDISKEN_UTILITIES_HPP_

#include "options.hpp"

#include <sdbus-c++/Types.h>

#include <chrono>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace utils {

/// Converts an array of array of bytes (D-Bus equivalent type: a{a{y}})
/// to a vector of strings.
///
/// @param aay The array of array of bytes.
[[nodiscard("Returns the converted vector")]]
auto ConvertArrayArrayByte(const std::vector<std::vector<uint8_t>>& aay)
    -> std::vector<std::string>;

/// Checks if the string view has a non-zero value.
///
/// @param sv Typical string view.
///
/// @return True if string view is non-empty (regardless of the original
/// string's content), and its content contains characters other than '0'.
constexpr auto NonZero(std::string_view sv) -> bool;

/// Checks if the environment variable is defined and has a non-zero value.
///
/// @param var Name of the environment variable.
///
/// @return True if the environment variable is defined and is non-zero.
auto NonZeroEnvironmentVariable(const std::string& var) -> bool;

}  // namespace utils

namespace notify {

using namespace std::chrono_literals;

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

/// Send a desktop notification.
///
/// @param notification Notification.
///
/// @return Successfully sent the notification.
auto Notify(const Notification& notif) -> bool;

}  // namespace notify

#endif  // UDISKEN_UTILITIES_HPP_
