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

/// Manager proxy: bridges interfaces to other proxies.

#include "udisks_manager.hpp"

#include "udisks_drive.hpp"
#include "udisks_filesystem.hpp"
#include "udisks_globals.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>

#include <cstddef>
#include <iostream>
#include <map>
#include <memory>
#include <print>
#include <vector>

namespace udisken {

UdisksManager::UdisksManager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection, sdbus::ServiceName{kInterfaceName},
                      sdbus::ObjectPath{kObjectPath}) {
  for (auto managed_objects = GetManagedObjects();
       const auto& [object_path, interfaces_and_properties] : managed_objects) {
    onInterfacesAdded(object_path, interfaces_and_properties);
  }
  registerProxy();
}

void UdisksManager::onInterfacesAdded(
    const sdbus::ObjectPath& object_path,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfaces_and_properties) {
  // TODO(xlacroixx): debug log
  std::println("Object {}", object_path.c_str());
  for (const auto& [interface, properties] : interfaces_and_properties) {
    std::println("- {}", interface.c_str());
    for (const auto& [member_name, value] : properties) {
      std::println("\t{}: {}", member_name.c_str(), value.peekValueType());
    }
  }
}

void UdisksManager::onInterfacesRemoved(
    const sdbus::ObjectPath& object_path,
    const std::vector<sdbus::InterfaceName>& interfaces) {
  for (const auto& interface : interfaces) {
    if (interface == UdisksFilesystem::INTERFACE_NAME) {
      // Such filesystem may have disappeared without being unmounted, either by
      // udisken or an external source. Unfortunately there is no way to check
      // this.
      //
      // Erases no key if udisken unmounted the filesystem.
      auto keys_erased = filesystems_.erase(object_path);

      if (keys_erased >= 1) {
        std::println(std::cerr, "{} disappeared", object_path);
      }
    }
  }
}

}  // namespace udisken
