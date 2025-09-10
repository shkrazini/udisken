// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <blackma9ick@proton.me>
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Copyright (C) 2025 Sofian-Hedi Krazini
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <https://www.gnu.org/licenses/>.

/// Contains the (auto)mounting logic itself.

#ifndef UDISKEN_MOUNT_HPP_
#define UDISKEN_MOUNT_HPP_

#include "udisks.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <string>
#include <vector>

namespace mount {

using MountPoints = std::vector<std::string>;

/// Retrieves mount points from a filesystem and converts them to standard
/// library types.
///
/// @param fs Reference to an UDisks Filesystem proxy.
///
/// @return List of strings representing the filesystem's mount points.
MountPoints GetMountPoints(
    org::freedesktop::UDisks2::proxies::UdisksFilesystem& fs);
/// Log mount points as verbose output.
///
/// @param mnt_points List of strings representing a filesystem's mount points.
void DebugMountPoints(const MountPoints& mnt_points);

/// Try to mount a block device's filesystem, to be used when automatically
/// mounting.
///
/// @return Path to mount point after mounting, or nothing if the
/// filesystem is already mounted somewhere.
///
/// @throws sdbus::Error Error returned by UDisks if automounting
/// failed. Does not throw if filesystem is already mounted somewhere.
auto TryAutomount(objects::BlockDevice& blk_device)
    -> std::optional<std::string>;

}  // namespace mount

#endif  // UDISKEN_MOUNT_HPP_
