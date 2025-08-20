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

#include <sdbus-c++/Types.h>

#include <cstdint>
#include <string>
#include <vector>

namespace utils {

/// Converts an array of array of bytes (D-Bus equivalent type: a{a{y}})
/// to a vector of strings.
///
/// @param aay The array of array of bytes.
[[nodiscard("Creates a converted vector")]]
auto ConvertArrayArrayByte(const std::vector<std::vector<uint8_t>>& aay)
    -> std::vector<std::string>;

/// Thin struct containing the essential properties of a libnotify
/// Notification that can be sent to desktop.
struct Notification {
  std::string summary;
  std::string body;
  std::string icon;
};

/// Send a desktop notification.
///
/// @param notification Notification.
///
/// @return Successfully sent the notification.
auto Notify(const Notification& notification) -> bool;

/// Checks if the string view has a non-zero value.
///
/// @param sv Typical string view.
///
/// @return True if string view is non-empty (regardless of the original
/// string's content), and its content contains characters other than '0'.
[[nodiscard]] constexpr auto NonZero(std::string_view sv) -> bool;

/// Checks if the environment variable is defined and has a non-zero value.
///
/// @param var Name of the environment variable.
///
/// @return True if the environment variable is defined and is non-zero.
[[nodiscard]] auto NonZeroEnvironmentVariable(const std::string& var) -> bool;

}  // namespace utils

#endif  // UDISKEN_UTILITIES_HPP_
