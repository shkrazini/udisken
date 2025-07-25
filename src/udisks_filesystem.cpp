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

namespace udisken {

// TODO(xlacroixx): refactor the mount + check into (member) function.
UdisksFilesystem::UdisksFilesystem(sdbus::IConnection& connection,
                                   const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName(kInterfaceName),
                      object_path) {
  if (!MountPoints().empty()) {
    // Mount points already exist, so no need to automount.

    registerProxy();

    return;
  }

  // TODO(xlacroixx): check whether we are allowed to automount.
  mount_path_ = Mount({});

  std::println(std::cerr, "Mounted {} at {}", getProxy().getObjectPath(),
               mount_path_);

  registerProxy();
}

// FIXME(xlacroixx): currently this destructor will fail if the device is busy.
// To prevent that, we need to Unmount using the `force` option.
// Preferably this destructor should be avoided!
UdisksFilesystem::~UdisksFilesystem() noexcept {
  Unmount({});

  std::println(std::cerr, "Unmounted {} at {}", getProxy().getObjectPath(),
               mount_path_);

  unregisterProxy();
}

}  // namespace udisken
