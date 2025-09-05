// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <blackma9ick@proton.me>
// SPDX-License-Identifier: Apache-2.0
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
