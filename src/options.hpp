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

/// Options and status variables for UDISKEN.

#ifndef UDISKEN_OPTIONS_HPP_
#define UDISKEN_OPTIONS_HPP_

#include <sdbus-c++/Types.h>

namespace globals {

/// Name of the program, so, "UDISKEN" pretty much.
constexpr auto kAppName = UDISKEN_NAME;
/// Version of the program.
constexpr auto kAppVer = UDISKEN_VERSION;

/// Whether UDISKEN was compiled in debug mode.
#ifdef NDEBUG
constexpr bool kDebug{false};
#else
constexpr bool kDebug{true};
#endif

/// Whether desktop notifications were enabled when building.
#ifdef FEATURE_NOTIFY
constexpr bool kNotify{true};
#else
constexpr bool kNotify{false};
#endif

}  // namespace globals

namespace options {

/// Should UDISKEN automount.
///
/// @return False if UDISKEN_NO_AUTOMOUNT is non-zero. True otherwise.
[[nodiscard]] auto ShouldMount() -> bool;

/// Should UDISKEN send desktop notifications.
///
/// @return False if UDISKEN_NO_NOTIFY is non-zero. True otherwise.
[[nodiscard]] auto ShouldNotify() -> bool;

}  // namespace options

#endif  // UDISKEN_OPTIONS_HPP_
