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

/// Manager proxies for UDisks interfaces.

#include "objects.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <map>
#include <vector>

#ifndef UDISKEN_COMMON_MANAGERS_HPP_
#define UDISKEN_COMMON_MANAGERS_HPP_

namespace managers {

namespace udisks = org::freedesktop::UDisks2;

/// UDisks top-level manager singleton object.
class UdisksManager final
    : public sdbus::ProxyInterfaces<udisks::Manager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksManager(sdbus::IConnection& connection);

  UdisksManager(const UdisksManager&) = delete;
  UdisksManager(UdisksManager&&) = delete;
  auto operator=(const UdisksManager&) -> UdisksManager& = delete;
  auto operator=(UdisksManager&&) -> UdisksManager& = delete;

  ~UdisksManager() noexcept { unregisterProxy(); }

  /// Object path to the manager singleton.
  // TODO(blackma9ick): construct this with globals::kObjectPath?
  static constexpr auto kObjectPath{"/org/freedesktop/UDisks2/Manager"};
};

/// Bridges the UDisks ObjectManager and interfaces.
class UdisksObjectManager final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksObjectManager(sdbus::IConnection& connection);

  UdisksObjectManager(UdisksObjectManager&&) = delete;
  UdisksObjectManager(const UdisksObjectManager&) = delete;
  auto operator=(UdisksObjectManager&&) -> UdisksObjectManager& = delete;
  auto operator=(const UdisksObjectManager&) -> UdisksObjectManager& = delete;

  ~UdisksObjectManager() noexcept { unregisterProxy(); }

 private:
  void onInterfacesAdded(
      const sdbus::ObjectPath& object_path,
      const std::map<sdbus::InterfaceName,
                     std::map<sdbus::PropertyName, sdbus::Variant>>&
          interfaces_and_properties) final;

  void onInterfacesRemoved(
      [[maybe_unused]] const sdbus::ObjectPath& object_path,
      [[maybe_unused]] const std::vector<sdbus::InterfaceName>& interfaces)
      final;

  std::map<sdbus::ObjectPath, objects::BlockDevice> block_devices_;
};

}  // namespace managers

#endif  // UDISKEN_COMMON_MANAGERS_HPP_
