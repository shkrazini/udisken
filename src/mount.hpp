// UDISKEN: A small Linux automounter.
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

auto GetMountPoints(interfaces::UdisksFilesystem& fs) -> MountPoints;
void PrintMountPoints(const MountPoints& mnt_points);

/// Automount.
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
