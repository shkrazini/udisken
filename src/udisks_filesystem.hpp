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

#ifndef UDISKEN_UDISKS_FILESYSTEM_HPP_
#define UDISKEN_UDISKS_FILESYSTEM_HPP_

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <string>
#include <vector>

namespace udisken {

namespace udisks = org::freedesktop::UDisks2;

class UdisksFilesystem final
    : public sdbus::ProxyInterfaces<udisks::Filesystem_proxy> {
 public:
  UdisksFilesystem(sdbus::IConnection& connection,
                   const sdbus::ObjectPath& object_path);

  UdisksFilesystem(UdisksFilesystem&&) = delete;
  UdisksFilesystem(const UdisksFilesystem&) = delete;
  auto operator=(UdisksFilesystem&&) -> UdisksFilesystem& = delete;
  auto operator=(const UdisksFilesystem&) -> UdisksFilesystem& = delete;

  ~UdisksFilesystem() noexcept;

 private:
  // TODO(xlacroixx): check whether we are allowed to automount.
  /// @brief Automount, if udisken is configured to do so.
  ///
  /// @return Path to the mount point after mounting, or nothing if the
  /// filesystem is already mounted somewhere.
  auto Automount() -> std::vector<std::string>;

  /// @brief List of mount paths for this filesystem.
  /// Guaranteed to have at least one element.
  std::vector<std::string> mount_paths_{};
};

}  // namespace udisken

#endif  // UDISKEN_UDISKS_FILESYSTEM_HPP_

// TEST(xlacroixx): constructing an UdisksFilesystem mounts the filesystem, if
// automounting is enabled; do not fail if filesystem is already mounted (e.g.
// by user)
// TEST(xlacroixx): destructing an UdisksFilesystem unmounts the filesystem, if
// automounting if enabled; do not fail if filesystem is already unmounted (e.g.
// by external program).
