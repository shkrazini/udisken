// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <blackma9ick@proton.me>
// SPDX-License-Identifier: Apache-2.0
//
// Copyright (C) 2025 Sofian-Hedi Krazini
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
