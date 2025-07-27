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

#ifndef UDISKEN_UTILITIES_HPP_
#define UDISKEN_UTILITIES_HPP_

#include <sdbus-c++/Types.h>

#include <cstdint>
#include <string>
#include <vector>

namespace utils {

/// @class Notification
/// @brief Thin struct containing the essential properties of a libnotify
/// Notification that can be sent to desktop.
///
struct Notification {
  std::string summary;
  std::string body;
  std::string icon;
};

/// @brief Send a desktop notification.
///
/// @param notification Notification.
/// @return Successfully sent the notification.
auto Notify(const Notification& notification) -> bool;

}  // namespace utils

namespace conversions {

/// Converts an array of array of bytes (D-Bus equivalent type: a{a{y}})
/// to a vector of strings.
///
/// @param aay The array of array of bytes.
[[nodiscard("Creates moved-to converted vector")]]
auto ConvertArrayArrayByte(const std::vector<std::vector<uint8_t>>& aay)
    -> std::vector<std::string>;

}  // namespace conversions

#endif  // UDISKEN_UTILITIES_HPP_
