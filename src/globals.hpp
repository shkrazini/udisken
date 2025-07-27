// UDISKEN: Linux drive automounter for the impatient.
// Copyright (C) 2025 xlacroixx
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

/// Global variables for UDISKEN and UDisks D-Bus API.

#ifndef UDISKEN_GLOBALS_HPP_
#define UDISKEN_GLOBALS_HPP_

namespace globals {

/// Whether UDISKEN was compiled in debug mode.
#ifdef NDEBUG
constexpr bool kDebug{false};
#else
constexpr bool kDebug{true};
#endif

/// Whether desktop notifications are enabled.
#ifdef FEATURE_NOTIFY
constexpr bool kNotify{true};
#else
constexpr bool kNotify{false};
#endif

constexpr auto kAppName = UDISKEN_NAME;
constexpr auto kInterfaceName = "org.freedesktop.UDisks2";
constexpr auto kObjectPath = "/org/freedesktop/UDisks2";

}  // namespace globals

#endif  // UDISKEN_GLOBALS_HPP_
