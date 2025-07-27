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

#ifndef UDISKEN_UDISKS_MANAGER_HPP_
#define UDISKEN_UDISKS_MANAGER_HPP_

#include "udisks_filesystem.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <map>
#include <memory>
#include <vector>

namespace udisken {

/// Bridges the UDisks ObjectManager and interfaces.
class UdisksManager final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksManager(sdbus::IConnection& connection);

  UdisksManager(UdisksManager&&) = delete;
  UdisksManager(const UdisksManager&) = delete;
  auto operator=(UdisksManager&&) -> UdisksManager& = delete;
  auto operator=(const UdisksManager&) -> UdisksManager& = delete;

  ~UdisksManager() noexcept { unregisterProxy(); }

 private:
  void onInterfacesAdded(
      const sdbus::ObjectPath& object_path,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfaces_and_properties) final;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& object_path,
      const std::vector<sdbus::InterfaceName>& interfaces) final;

  /// Drive proxies handled by udisken. They always represent an object with at
  /// least one filesystem proxy.
  std::map<sdbus::ObjectPath, std::unique_ptr<UdisksDrive>> drives_{};
  /// Filesystem proxies handled by udisken. They always represent an object
  /// with at least one drive proxy.
  std::map<sdbus::ObjectPath, std::unique_ptr<UdisksFilesystem>> filesystems_{};
};

}  // namespace udisken

#endif  // UDISKEN_UDISKS_MANAGER_HPP_
