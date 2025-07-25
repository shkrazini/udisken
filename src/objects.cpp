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

/// Concrete objects implementing many UDisks interfaces.

#include "objects.hpp"

#include "interfaces.hpp"
#include "utilities.hpp"

#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-c++/udisks_errors.hpp>

#include <format>
#include <memory>
#include <optional>
#include <utility>

namespace objects {

BlockDevice::BlockDevice(
    const sdbus::ObjectPath& object_path,
    std::unique_ptr<interfaces::UdisksBlock> block,
    std::unique_ptr<interfaces::UdisksFilesystem> filesystem,
    std::unique_ptr<interfaces::UdisksLoop> loop,
    std::unique_ptr<interfaces::UdisksLoop> partition)
    : block_{std::move(block)},
      filesystem_{std::move(filesystem)},
      loop_{std::move(loop)},
      partition_{std::move(partition)} {
  if (!filesystem_) {
    spdlog::info("Not automounting {}: block device has no filesystem",
                 object_path.c_str());
    return;
  }

  if (block_->HintAuto()) {
    if (auto mnt_point = Automount(*filesystem_); mnt_point.has_value()) {
      spdlog::info("Automounted {}", *mnt_point);
      utils::Notification notif{
          .summary = "Mounted disk",
          .body = std::format("{} at {}", block_->HintName(), *mnt_point),
          .icon = "drive-removable-media"};
      utils::Notify(notif);

      return;
    }

    spdlog::debug("Not automounting {}: automount hint was false",
                  object_path_.c_str());
  }
}

}  // namespace objects
