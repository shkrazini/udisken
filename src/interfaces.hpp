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

/// Interfaces proxies and functions acting upon them.

#ifndef UDISKEN_INTERFACES_HPP_
#define UDISKEN_INTERFACES_HPP_

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <string>
#include <vector>

namespace interfaces {

namespace udisks = org::freedesktop::UDisks2;

/// Proxy to a UDisks block interface.
///
/// UdisksBlockDevice always implements this interface.
class UdisksBlock final : public sdbus::ProxyInterfaces<udisks::Block_proxy> {
 public:
  UdisksBlock(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksBlock(UdisksBlock&&) = delete;
  UdisksBlock(const UdisksBlock&) = delete;
  auto operator=(UdisksBlock&&) -> UdisksBlock& = delete;
  auto operator=(const UdisksBlock&) -> UdisksBlock& = delete;

  ~UdisksBlock() noexcept { unregisterProxy(); };
};

/// Proxy to a UDisks disk drive interface.
class UdisksDrive final : public sdbus::ProxyInterfaces<udisks::Drive_proxy> {
 public:
  UdisksDrive(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksDrive(const UdisksDrive&) = delete;
  UdisksDrive(UdisksDrive&&) = delete;
  auto operator=(const UdisksDrive&) -> UdisksDrive& = delete;
  auto operator=(UdisksDrive&&) -> UdisksDrive& = delete;

  ~UdisksDrive() noexcept { unregisterProxy(); }
};

/// Proxy to a UDisks mountable filesystem interface, contained in a
/// UdisksBlockDevice.
///
/// UdisksBlockDevice may implement this interface.
class UdisksFilesystem final
    : public sdbus::ProxyInterfaces<udisks::Filesystem_proxy> {
 public:
  /// Construct a filesystem proxy and execute actions on it, e.g.,
  /// automounting.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks Filesystem interface.
  UdisksFilesystem(sdbus::IConnection& connection,
                   const sdbus::ObjectPath& object_path);

  UdisksFilesystem(UdisksFilesystem&&) = delete;
  UdisksFilesystem(const UdisksFilesystem&) = delete;
  auto operator=(UdisksFilesystem&&) -> UdisksFilesystem& = delete;
  auto operator=(const UdisksFilesystem&) -> UdisksFilesystem& = delete;

  ~UdisksFilesystem() noexcept { unregisterProxy(); }
};

using MountPoints = std::vector<std::string>;

auto GetMountPoints(UdisksFilesystem& fs) -> MountPoints;
void PrintMountPoints(const MountPoints& mnt_points);

/// Proxy to a UDisks loop device interface.
///
/// UdisksBlockDevice may implement this interface.
class UdisksLoop : public sdbus::ProxyInterfaces<udisks::Loop_proxy> {
 public:
  UdisksLoop(sdbus::IConnection& connection,
             const sdbus::ObjectPath& objectPath);

  UdisksLoop(UdisksLoop&&) = delete;
  UdisksLoop(const UdisksLoop&) = delete;
  auto operator=(UdisksLoop&&) -> UdisksLoop& = delete;
  auto operator=(const UdisksLoop&) -> UdisksLoop& = delete;

  ~UdisksLoop() noexcept { unregisterProxy(); }
};

/// Proxy to a UDisks partition interface.
///
/// UdisksBlockDevice may implement this interface.
class UdisksPartition : public sdbus::ProxyInterfaces<udisks::Partition_proxy> {
 public:
  UdisksPartition(sdbus::IConnection& connection,
                  const sdbus::ObjectPath& objectPath);

  UdisksPartition(UdisksPartition&&) = delete;
  UdisksPartition(const UdisksPartition&) = delete;
  auto operator=(UdisksPartition&&) -> UdisksPartition& = delete;
  auto operator=(const UdisksPartition&) -> UdisksPartition& = delete;

  ~UdisksPartition() noexcept { unregisterProxy(); }
};

}  // namespace interfaces

#endif  // UDISKEN_INTERFACES_HPP_
