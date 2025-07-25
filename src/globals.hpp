// udisken: an automounter that stays quite quiet.
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

/// Global variables for UDisks D-Bus API.

#ifndef UDISKEN_GLOBALS_HPP_
#define UDISKEN_GLOBALS_HPP_

namespace globals {

static constexpr auto kInterfaceName = "org.freedesktop.UDisks2";
static constexpr auto kObjectPath = "/org/freedesktop/UDisks2";

}  // namespace globals

#endif  // UDISKEN_GLOBALS_HPP_
