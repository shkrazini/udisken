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

/// Concrete objects implementing many UDisks interfaces.

#include "objects.hpp"

#include "interfaces.hpp"
#include "utilities.hpp"

#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>

#include <format>
#include <memory>
#include <optional>
#include <utility>

namespace objects {

BlockDevice::BlockDevice(
    const sdbus::ObjectPath& object_path,
    std::unique_ptr<interfaces::UdisksBlock> block,
    std::unique_ptr<interfaces::UdisksFilesystem> filesystem)
    : object_path_{object_path},
      block_{std::move(block)},
      filesystem_{std::move(filesystem)} {
  if (block_->HintAuto()) {
    if (auto mnt_point = filesystem_->Automount(); mnt_point.has_value()) {
      spdlog::info("Automounted {}", *mnt_point);
      utils::Notification notif{
          .summary = "Mounted disk",
          .body = std::format("{} at {}", block_->HintName(), *mnt_point),
          .icon = "drive-removable-media"};
      utils::Notify(notif);
    }
  }

  spdlog::debug("Got hint to not automount {}", object_path_.c_str());
}

}  // namespace objects
