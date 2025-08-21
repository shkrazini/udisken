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

#include <sdbus-c++/Types.h>

#include <cstdint>
#include <ranges>
#include <string>
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

auto Notify([[maybe_unused]] const Notification& notification) -> bool {
  // TODO(blackma9ick): add open in FM feature.
  return false;
}

constexpr auto NonZero(std::string_view sv) -> bool {
  // NOLINTNEXTLINE(readability-static-accessed-through-instance)
  return !sv.empty() && sv.find_first_not_of("0") != sv.npos;
}

static_assert(NonZero("1"));
static_assert(NonZero("101"));
static_assert(NonZero("010"));
static_assert(!NonZero(""));
static_assert(!NonZero("0"));

auto NonZeroEnvironmentVariable(const std::string& var) -> bool {
  const auto var_value = std::getenv(var.c_str());
  return var_value != nullptr && NonZero(var_value);
}

}  // namespace utils
