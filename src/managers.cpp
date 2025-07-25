// UDISKEN: Linux drive automounter for the impatient.
// Copyright (C) BlackMa9ick
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

/// Manager proxy: bridges interfaces to objects.

#include "managers.hpp"

#include "globals.hpp"
#include "interfaces.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>

#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace managers {

UdisksManager::UdisksManager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection, sdbus::ServiceName{globals::kInterfaceName},
                      sdbus::ObjectPath{kObjectPath}) {
  registerProxy();
}

UdisksObjectManager::UdisksObjectManager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection, sdbus::ServiceName{globals::kInterfaceName},
                      sdbus::ObjectPath{globals::kObjectPath}) {
  for (auto managed_objects = GetManagedObjects();
       const auto& [object_path, interfaces_and_properties] : managed_objects) {
    onInterfacesAdded(object_path, interfaces_and_properties);
  }

  registerProxy();
}

void UdisksObjectManager::onInterfacesAdded(
    const sdbus::ObjectPath& object_path,
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&
        interfaces_and_properties) {
  spdlog::debug("New object: {}", object_path.c_str());

  if (interfaces_and_properties.contains(
          sdbus::InterfaceName{interfaces::UdisksBlock::INTERFACE_NAME}) &&
      interfaces_and_properties.contains(
          sdbus::InterfaceName{interfaces::UdisksFilesystem::INTERFACE_NAME})) {
    objects::BlockDevice blk_device{
        object_path,
        std::make_unique<interfaces::UdisksBlock>(getProxy().getConnection(),
                                                  object_path),
        std::make_unique<interfaces::UdisksFilesystem>(
            getProxy().getConnection(), object_path)};

    block_devices_.try_emplace(object_path, std::move(blk_device));

    spdlog::debug("Added Block device at {}", object_path.c_str());
  }
}

void UdisksObjectManager::onInterfacesRemoved(
    [[maybe_unused]] const sdbus::ObjectPath& object_path,
    [[maybe_unused]] const std::vector<sdbus::InterfaceName>& interfaces) {
  // for (const auto& interface : interfaces) {
  // }
}

}  // namespace managers
