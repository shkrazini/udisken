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

/// Interfaces proxies with some automatic methods executed.

#ifndef UDISKEN_INTERFACES_HPP_
#define UDISKEN_INTERFACES_HPP_

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <optional>
#include <string>
#include <vector>

namespace interfaces {

namespace udisks = org::freedesktop::UDisks2;

class UdisksBlock final : public sdbus::ProxyInterfaces<udisks::Block_proxy> {
 public:
  UdisksBlock(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksBlock(UdisksBlock&&) = delete;
  UdisksBlock(const UdisksBlock&) = delete;
  auto operator=(UdisksBlock&&) -> UdisksBlock& = delete;
  auto operator=(const UdisksBlock&) -> UdisksBlock& = delete;

  ~UdisksBlock() noexcept { unregisterProxy(); };

 private:
};

class UdisksDrive final : public sdbus::ProxyInterfaces<udisks::Drive_proxy> {
 public:
  UdisksDrive(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksDrive(const UdisksDrive&) = delete;
  UdisksDrive(UdisksDrive&&) = delete;
  auto operator=(const UdisksDrive&) -> UdisksDrive& = delete;
  auto operator=(UdisksDrive&&) -> UdisksDrive& = delete;

  ~UdisksDrive() noexcept { unregisterProxy(); }

 private:
};

/// Proxy to a UDisks Filesystem interface. Keep track of a filesystem and
/// (automatically) execute actions on it, e.g., automounting.
class UdisksFilesystem final
    : public sdbus::ProxyInterfaces<udisks::Filesystem_proxy> {
 public:
  /// Construct a filesystem proxy and execute actions on it, e.g.,
  /// automounting.
  ///
  /// @param connection System bus connection. Should be the same as used to
  /// construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks Filesystem interface.
  UdisksFilesystem(sdbus::IConnection& connection,
                   const sdbus::ObjectPath& object_path);

  UdisksFilesystem(UdisksFilesystem&&) = delete;
  UdisksFilesystem(const UdisksFilesystem&) = delete;
  auto operator=(UdisksFilesystem&&) -> UdisksFilesystem& = delete;
  auto operator=(const UdisksFilesystem&) -> UdisksFilesystem& = delete;

  ~UdisksFilesystem() noexcept { unregisterProxy(); }

  /// Automount, TODO(xlacroixx): if udisken is configured to do so.
  ///
  /// @return Path to mount point after mounting, or nothing if the filesystem
  /// is already mounted somewhere.
  /// mount_paths_ (private) may contain multiple paths before or after
  /// mounting.
  ///
  /// @throws sdbus::Error Error returned by UDisks if automounting failed.
  /// Does not throw if filesystem is already mounted somewhere.
  auto Automount() -> std::optional<std::string>;

 private:
  /// List of mount paths for this filesystem.
  /// Can be empty, if udisken cannot or will not mount this filesystem.
  std::vector<std::string> mount_points_;
};

}  // namespace interfaces

#endif  // UDISKEN_INTERFACES_HPP_
