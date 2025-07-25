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

#ifndef UDISKEN_FILESYSTEM_PROXY_HPP_
#define UDISKEN_FILESYSTEM_PROXY_HPP_

#include "udisks_proxy.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>

#include <string>

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
  static constexpr auto kInterfaceName = "org.freedesktop.UDisks2";

  // TODO(xlacroixx): should be public?
  std::string mount_path_;
};

}  // namespace udisken

#endif  // UDISKEN_FILESYSTEM_PROXY_HPP_

// TEST(xlacroixx): constructing an UdisksFilesystem mounts the filesystem, if
// automounting is enabled; do not fail if filesystem is already mounted (e.g.
// by user)
// TEST(xlacroixx): destructing an UdisksFilesystem unmounts the filesystem, if
// automounting if enabled; do not fail if filesystem is already unmounted (e.g.
// by external program).
