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

/// Options and status variables for UDISKEN.

#ifndef UDISKEN_OPTIONS_HPP_
#define UDISKEN_OPTIONS_HPP_

#include <sdbus-c++/Types.h>

#include <string>

namespace globals {

/// Name of the program, as seen by other programs and machines;
/// that's pretty much "udisken".
constexpr std::string kAppName{UDISKEN_NAME};
/// Name of the program, as it should be presented in interfaces and logs;
/// that's pretty much "UDISKEN".
constexpr std::string kAppNameUi{UDISKEN_NAME_UI};
/// Version of the program.
constexpr std::string kAppVer{UDISKEN_VERSION};

/// Whether UDISKEN was compiled in debug mode.
#ifdef NDEBUG
constexpr bool kDebug{false};
#else
constexpr bool kDebug{true};
#endif

}  // namespace globals

namespace options {

struct Options {
  bool notify{true};
};

/// Are desktop notifications enabled by the environment.
///
/// @return False if UDISKEN_NO_NOTIFY is non-zero. True otherwise.
auto NotifyEnabled() -> bool;

}  // namespace options

#endif  // UDISKEN_OPTIONS_HPP_
