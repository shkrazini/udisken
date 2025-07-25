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

/// Proxies with some automatic methods executed.

#include "proxies.hpp"

#include "conversions.hpp"
#include "globals.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-c++/udisks_errors.hpp>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace proxies {

UdisksBlock::UdisksBlock(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName{globals::kInterfaceName},
                      object_path) {
  registerProxy();
}

UdisksDrive::UdisksDrive(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName(globals::kInterfaceName),
                      object_path) {
  registerProxy();
}

UdisksFilesystem::UdisksFilesystem(sdbus::IConnection& connection,
                                   const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName(globals::kInterfaceName),
                      object_path) {
  registerProxy();
}

auto UdisksFilesystem::Automount() -> std::optional<std::string> {
  // If mount points already exist, no need to automount it...
  // TODO(xlacroixx): ...unless other paths are given to udisken and it should
  // mount?)
  if (!MountPoints().empty()) {
    mount_points_ = conversions::ConvertArrayArrayByte(MountPoints());

    return std::nullopt;
  }

  try {
    auto mnt_point = Mount({});
    spdlog::info("Automounted {}", mnt_point);

    if (std::ranges::find(mount_points_, mnt_point) != mount_points_.end()) {
      spdlog::warn(
          "UDisks succesfully mounted, but it already had a mount point");

      return std::nullopt;
    }

    mount_points_.push_back(mnt_point);
    spdlog::debug("Current mount points:");
    for (const auto& current_mnt_points : mount_points_) {
      spdlog::debug("- {}", current_mnt_points);
    }

    return mnt_point;
  } catch (const sdbus::Error& e) {
    if (e.getName() ==
        std::get<udisks::UdisksErrors::kUdisksErrorAlreadyMounted>(
            udisks::udisks_error_names)) {
      // Should have been caught at the guard at the beginning of the function,
      // so this is weird.

      mount_points_ = conversions::ConvertArrayArrayByte(MountPoints());

      spdlog::warn(
          "{} is already mounted but UDisks initially returned no mount "
          "paths;\nMount paths after asking again:",
          getProxy().getObjectPath().c_str());
      for (const auto& mount_point : mount_points_) {
        spdlog::warn("- {}", mount_point);
      }

      return std::nullopt;
    }

    spdlog::error("Failed to automount:\n({}) {}", e.getName().c_str(),
                  e.getMessage());

    throw;
  }
}

}  // namespace proxies

// TEST(xlacroixx): constructing an UdisksFilesystem mounts the filesystem, if
// automounting is enabled; do not fail if filesystem is already mounted (e.g.
// by user)
// TEST(xlacroixx): destructing an UdisksFilesystem unmounts the filesystem, if
// automounting if enabled; do not fail if filesystem is already unmounted (e.g.
// by external program).
