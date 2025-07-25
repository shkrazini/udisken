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

#include "interfaces.hpp"

#include "conversions.hpp"
#include "globals.hpp"

#include <libnotify/notification.h>
#include <sdbus-c++/Error.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-c++/udisks_errors.hpp>

#include <string>

namespace {

void PrintMountPoints(std::span<const std::string> mount_points) {
  spdlog::debug("Current mount points:");
  for (const auto& current_mnt_points : mount_points) {
    spdlog::debug("- {}", current_mnt_points);
  }
}

}  // namespace

namespace interfaces {

UdisksBlock::UdisksBlock(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName{globals::kInterfaceName},
                      object_path) {
  registerProxy();
}

UdisksDrive::UdisksDrive(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, globals::kServiceName, object_path) {
  registerProxy();
}

UdisksFilesystem::UdisksFilesystem(sdbus::IConnection& connection,
                                   const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, globals::kServiceName, object_path) {
  registerProxy();
}

UdisksPartition::UdisksPartition(sdbus::IConnection& connection,
                                 const sdbus::ObjectPath& objectPath)
    : ProxyInterfaces<Partition_proxy>(connection, globals::kServiceName,
                                       objectPath) {}

auto Automount(interfaces::UdisksFilesystem& fs) -> std::optional<std::string> {
  // If mount points already exist, no need to automount it...
  // TODO(blackma9ick): ...unless other paths are given to UDISKEN and it
  // should mount?)
  if (!fs.MountPoints().empty()) {
    return std::nullopt;
  }

  auto mount_points = conversions::ConvertArrayArrayByte(fs.MountPoints());

  try {
    auto mnt_point = fs.Mount({});

    PrintMountPoints(mount_points);

    return mnt_point;
  } catch (const sdbus::Error& e) {
    if (e.getName() ==
        std::get<udisks::UdisksErrors::kUdisksErrorAlreadyMounted>(
            udisks::udisks_error_names)) {
      // Should have been caught at the guard at the beginning
      // of the function, so this is weird.

      spdlog::warn(
          "{} is already mounted but UDisks initially returned no mount paths;",
          fs.getProxy().getObjectPath().c_str());
      PrintMountPoints(mount_points);

      return std::nullopt;
    }

    spdlog::error("Failed to automount:\n({}) {}", e.getName().c_str(),
                  e.getMessage());

    throw;
  }
}

}  // namespace interfaces

// TEST(blackma9ick): constructing an UdisksFilesystem mounts the filesystem, if
// automounting is enabled; do not fail if filesystem is already mounted (e.g.
// by user)
// TEST(blackma9ick): destructing an UdisksFilesystem unmounts the filesystem,
// if automounting if enabled; do not fail if filesystem is already unmounted
// (e.g. by external program).
