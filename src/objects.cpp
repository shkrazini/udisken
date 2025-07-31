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

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace objects {

Drive::Drive(std::unique_ptr<interfaces::UdisksDrive> drive)
    : drive_{std::move(drive)} {
  if (!drive_) {
    throw std::invalid_argument("drive pointer must not be null");
  }
}

auto Drive::ObjectPath() const -> const sdbus::ObjectPath& {
  return drive_->getProxy().getObjectPath();
}

auto Drive::drive() -> interfaces::UdisksDrive& { return *drive_; }

BlockDevice::BlockDevice(
    std::unique_ptr<interfaces::UdisksBlock> block,
    std::unique_ptr<interfaces::UdisksFilesystem> filesystem,
    std::unique_ptr<interfaces::UdisksLoop> loop,
    std::unique_ptr<interfaces::UdisksLoop> partition)
    : block_{std::move(block)},
      filesystem_{std::move(filesystem)},
      loop_{std::move(loop)},
      partition_{std::move(partition)} {
  if (!block_) {
    throw std::invalid_argument("block pointer must not be null");
  }

  if (block_->HasDrive()) {
    // TODO(blackma9ick): make one of the constructors prettier.
    drive_ = std::make_unique<Drive>(std::make_unique<interfaces::UdisksDrive>(
        block_->getProxy().getConnection(), block_->Drive()));
  }
}

[[nodiscard]] auto BlockDevice::ObjectPath() const -> const sdbus::ObjectPath& {
  return block_->getProxy().getObjectPath();
}

namespace {

namespace udisks = org::freedesktop::UDisks2;

auto Automount(interfaces::UdisksFilesystem& fs) -> std::optional<std::string> {
  try {
    auto mnt_point = fs.Mount({});

    interfaces::PrintMountPoints(interfaces::GetMountPoints(fs));

    return mnt_point;
  } catch (const sdbus::Error& e) {
    if (e.getName() ==
        udisks::ErrorName(udisks::UdisksErrors::kUdisksErrorAlreadyMounted)) {
      spdlog::warn(
          "{} is already mounted but UDisks initially returned no mount paths;",
          fs.getProxy().getObjectPath().c_str());
      interfaces::PrintMountPoints(interfaces::GetMountPoints(fs));

      return std::nullopt;
    }

    spdlog::error("Failed to automount:\n({}) {}", e.getName().c_str(),
                  e.getMessage());

    throw;
  }
}

}  // namespace

auto TryAutomount(objects::BlockDevice& blk_device)
    -> std::optional<std::string> {
  auto print_not_automounting = [&blk_device](std::string_view reason) {
    spdlog::debug("Not automounting {}: {}", blk_device.ObjectPath().c_str(),
                  reason);
  };

  if (!blk_device.block().HintAuto()) {
    print_not_automounting("automount hint was false");

    return std::nullopt;
  }

  // Can there even be a filesystem if HintAuto was false?
  if (!blk_device.HasFilesystem()) {
    print_not_automounting("no filesystem found");

    return std::nullopt;
  }

  // If mount points already exist, no need to automount it...
  // TODO(blackma9ick): ...unless other paths are given to UDISKEN and it
  // should mount?)
  if (!blk_device.filesystem().MountPoints().empty()) {
    print_not_automounting("already mounted");

    return std::nullopt;
  }

  auto mnt_point = Automount(blk_device.filesystem());

  if (mnt_point.has_value()) {
    spdlog::info("Automounted {}", *mnt_point);
    utils::Notification notif{
        .summary = "Mounted disk",
        .body =
            std::format("{} at {}", blk_device.block().HintName(), *mnt_point),
        .icon = "drive-removable-media"};
    utils::Notify(notif);
  }

  return mnt_point;
}

auto BlockDevice::filesystem() -> interfaces::UdisksFilesystem& {
  if (!HasFilesystem()) {
    throw std::logic_error("interface does not exist");
  }

  return *filesystem_;
}

auto BlockDevice::loop() -> interfaces::UdisksLoop& {
  if (!HasLoop()) {
    throw std::logic_error("interface does not exist");
  }

  return *loop_;
}

auto BlockDevice::partition() -> interfaces::UdisksLoop& {
  if (!HasPartition()) {
    throw std::logic_error("interface does not exist");
  }

  return *partition_;
}

}  // namespace objects
