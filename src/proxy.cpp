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

#include "proxy.hpp"

#include "filesystem_proxy.hpp"

#include <sdbus-c++/Types.h>
#include <sdbus-c++/sdbus-c++.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <cstddef>
#include <iostream>
#include <map>
#include <print>
#include <vector>

namespace udisken {

UdisksManager::UdisksManager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection, sdbus::ServiceName{kInterfaceName},
                      sdbus::ObjectPath{kObjectPath}) {
  // TODO(xlacroixx): manage existing objects.
  registerProxy();
}

void UdisksManager::onInterfacesAdded(
    const sdbus::ObjectPath& object_path,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfaces_and_properties) {
  for (const auto& [interface, properties] : interfaces_and_properties) {
    if (interface == UdisksFilesystem::INTERFACE_NAME) {
      // XXX(xlacroixx): is pre-existing key even a possibility?
      filesystems_.try_emplace(
          object_path,
          UdisksFilesystem(getProxy().getConnection(), object_path));
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

      std::size_t keys_erased{filesystems_.erase(object_path)};

      if (keys_erased >= 1) {
        std::println(std::cerr, "{} disappeared", object_path);
      }
    }
  }
}

}  // namespace udisken
