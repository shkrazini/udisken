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

/// Options and status variables for UDISKEN.

#ifndef UDISKEN_OPTIONS_HPP_
#define UDISKEN_OPTIONS_HPP_

#include <sdbus-c++/Types.h>

#include <string>

/// Status options enabled at compile-time for UDISKEN.
namespace globals {

/// Name of the program, as seen by other programs and machines;
/// that's pretty much "udisken".
constexpr std::string kAppName{UDISKEN_NAME};
/// Name of the program, as it should be presented in interfaces and logs;
/// that's pretty much "UDISKEN".
constexpr std::string kAppNameUi{UDISKEN_NAME_UI};
/// Version of the program.
constexpr std::string kAppVersion{UDISKEN_VERSION};

/// Whether UDISKEN was compiled in debug mode.
#ifdef NDEBUG
constexpr bool kDebug{false};
#else
constexpr bool kDebug{true};
#endif

}  // namespace globals

/// Options enabled at runtime for UDISKEN.
namespace options {

/// Checks if the string view has a non-zero value.
///
/// @param sv Typical string view.
///
/// @return True if string view is non-empty (regardless of the original
/// string's content), and its content contains characters other than '0'.
constexpr bool NonZero(std::string_view sv);

/// Checks if the environment variable is defined and has a non-zero value.
///
/// @param var Name of the environment variable.
///
/// @return True if the environment variable is defined and is non-zero.
bool NonZeroEnvVar(const std::string& var);

/// Struct housing enabled options for runtime features.
/// Use this to know which functions you are allowed to perform at runtime.
struct Options {
  /// Should we send Desktop notifications?
  bool notify{true};
};

/// Are desktop notifications enabled by the environment?
///
/// @return False if UDISKEN_NO_NOTIFY is non-zero. True otherwise.
bool NotifyEnabled();

}  // namespace options

#endif  // UDISKEN_OPTIONS_HPP_
