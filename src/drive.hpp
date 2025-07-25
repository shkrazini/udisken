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

/// Drive proxy: information (e.g. model, size) and methods on an existing
/// drive.

#ifndef UDISKEN_DRIVE_HPP_
#define UDISKEN_DRIVE_HPP_

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

namespace proxies {

namespace udisks = org::freedesktop::UDisks2;

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

}  // namespace proxies

#endif  // UDISKEN_DRIVE_HPP_
