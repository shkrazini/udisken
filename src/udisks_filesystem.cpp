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

// UDisks filesystem proxy: automounting

#include "udisks_filesystem.hpp"

#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace udisken {

// FIXME(xlacroixx): currently this destructor will fail if the device is busy.
// To prevent that, we need to Unmount using the `force` option.
// Preferably this destructor should be avoided!
UdisksFilesystem::~UdisksFilesystem() noexcept {
  Unmount({});

  // TODO(xlacroixx): iterate over all mount points
  std::println(std::cerr, "Unmounted {} at {}", getProxy().getObjectPath(),
               mount_paths_[0]);

  unregisterProxy();
}

auto UdisksFilesystem::Automount() -> std::vector<std::string> {
  // TODO(xlacroixx): re-enable the check.
  // If mount points already exist, no need to automount it...
  // TODO(xlacroixx): ...unless other paths are given to udisken and it should
  // mount?)
  // if (!MountPoints().empty()) {
  //   return conversions::AAYToVectorString(MountPoints());
  // }

  const auto mount_path = Mount({});

  // FIXME(xlacroixx): getObjectPath() returns a literal array of bytes. Convert
  // that. See run.log in data/
  std::println(std::cerr, "Mounted {} at {}", getProxy().getObjectPath(),
               mount_path);

  return {mount_path};
}

}  // namespace udisken
