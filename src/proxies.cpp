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

/// Proxies with some automatic methods executed.

#include "proxies.hpp"

#include "conversions.hpp"
#include "globals.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>

#include <iostream>
#include <print>
#include <string>
#include <vector>

namespace proxies {

UdisksBlock::UdisksBlock(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName{globals::kInterfaceName},
                      object_path) {
  registerProxy();
}

UdisksDrive::UdisksDrive(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName(globals::kInterfaceName),
                      object_path) {
  registerProxy();
}

UdisksFilesystem::UdisksFilesystem(sdbus::IConnection& connection,
                                   const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName(globals::kInterfaceName),
                      object_path),
      mount_paths_{Automount()} {
  registerProxy();
}

auto UdisksFilesystem::Automount() -> std::vector<std::string> {
  // If mount points already exist, no need to automount it...
  // TODO(xlacroixx): ...unless other paths are given to udisken and it should
  // mount?)
  if (!MountPoints().empty()) {
    return conversions::ConvertArrayArrayByte(MountPoints());
  }

  try {
    const auto mount_path = Mount({});

    std::println(std::cerr, "Mounted {} at {}",
                 getProxy().getObjectPath().c_str(), mount_path);

    return {mount_path};
  } catch (const sdbus::Error& e) {
    // TODO(xlacroixx): handle what we can, throw the rest.
    std::println("Failed to automount: [{}] {}", e.getName().c_str(),
                 e.getMessage());

    return {};
  }
}

}  // namespace proxies

// TEST(xlacroixx): constructing an UdisksFilesystem mounts the filesystem, if
// automounting is enabled; do not fail if filesystem is already mounted (e.g.
// by user)
// TEST(xlacroixx): destructing an UdisksFilesystem unmounts the filesystem, if
// automounting if enabled; do not fail if filesystem is already unmounted (e.g.
// by external program).
