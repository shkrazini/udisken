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

// UDisks manager proxy: handles changes on interfaces and properties.

#ifndef UDISKEN_UDISKS_MANAGER_HPP_
#define UDISKEN_UDISKS_MANAGER_HPP_

#include "udisks_filesystem.hpp"

#include <sdbus-c++/sdbus-c++.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <map>
#include <vector>

namespace udisken {

class UdisksManager final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
 public:
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

  static constexpr auto kInterfaceName = "org.freedesktop.UDisks2";
  static constexpr auto kObjectPath = "/org/freedesktop/UDisks2";

  std::map<sdbus::ObjectPath, UdisksFilesystem> filesystems_;
};

}  // namespace udisken

#endif  // UDISKEN_UDISKS_MANAGER_HPP_
