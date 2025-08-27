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

#include "options.hpp"

#include "spdlog/spdlog.h"

#include <cstdlib>
#include <string>

namespace options {

constexpr bool NonZero(std::string_view sv) {
  return !sv.empty() && sv.find_first_not_of("0") != std::string_view::npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

bool NonZeroEnvVar(const std::string& var) {
  auto* const var_value{std::getenv(var.c_str())};
  return var_value != nullptr && NonZero(var_value);
}

bool NotifyEnabled() {
  if (NonZeroEnvVar("UDISKEN_NO_NOTIFY")) {
    spdlog::debug("Notifications disabled by environment.");

    return false;
  }

  return true;
}

}  // namespace options
