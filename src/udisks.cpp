// UDISKEN: A small Linux automounter.
//
// SPDX-FileCopyrightText: 2025 Sofian-Hedi Krazini <sofian-hedi.krazini@proton.me>
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

/// UDisks objects, interface proxies, managers and related functions.
/// Contains the entrypoints to most of UDISKEN's logic, such as automounting.

#include "udisks.hpp"

#include "mount.hpp"
#include "options.hpp"

#include <sdbus-c++/Error.h>
#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>
#include <udisks-sdbus-cpp/udisks_errors.hpp>

#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace objects {

Drive::Drive(std::unique_ptr<udisks_sd::proxy_wrappers::UdisksDrive> drive)
    : drive_{std::move(drive)} {
  if (!drive_) {
    throw std::invalid_argument("drive pointer must not be null");
  }
}

const sdbus::ObjectPath& Drive::ObjectPath() const {
  return drive_->getProxy().getObjectPath();
}

auto Drive::GetDrive() -> udisks_sd::proxy_wrappers::UdisksDrive& {
  return *drive_;
}

BlockDevice::BlockDevice(
    std::unique_ptr<udisks_sd::proxy_wrappers::UdisksBlock> block,
    std::unique_ptr<udisks_sd::proxy_wrappers::UdisksFilesystem> filesystem,
    std::unique_ptr<udisks_sd::proxy_wrappers::UdisksLoop> loop,
    std::unique_ptr<udisks_sd::proxy_wrappers::UdisksPartition> partition)
    : block_{std::move(block)},
      filesystem_{std::move(filesystem)},
      loop_{std::move(loop)},
      partition_{std::move(partition)} {
  if (!block_) {
    throw std::invalid_argument("block pointer must not be null");
  }

  if (block_->Drive() != udisks::kEmptyObjectPath) {
    drive_ = std::make_unique<Drive>(
        std::make_unique<udisks_sd::proxy_wrappers::UdisksDrive>(
            block_->getProxy().getConnection(), block_->Drive()));
  }
}

const sdbus::ObjectPath& BlockDevice::ObjectPath() const {
  return block_->getProxy().getObjectPath();
}

auto BlockDevice::Filesystem() -> udisks_sd::proxy_wrappers::UdisksFilesystem& {
  if (!HasFilesystem()) {
    throw std::logic_error("object does not implement interface");
  }

  return *filesystem_;
}

auto BlockDevice::Loop() -> udisks_sd::proxy_wrappers::UdisksLoop& {
  if (!HasLoop()) {
    throw std::logic_error("object does not implement interface");
  }

  return *loop_;
}

auto BlockDevice::Partition() -> udisks_sd::proxy_wrappers::UdisksPartition& {
  if (!HasPartition()) {
    throw std::logic_error("object does not implement interface");
  }

  return *partition_;
}

}  // namespace objects

namespace managers {

UdisksManager::UdisksManager(sdbus::IConnection& connection)
    : ProxyInterfaces(connection, sdbus::ServiceName{udisks::kInterfaceName},
                      sdbus::ObjectPath{kObjectPath}) {
  registerProxy();
}

UdisksObjectManager::UdisksObjectManager(sdbus::IConnection& connection,
                                         options::Options options)
    : ProxyInterfaces(connection, sdbus::ServiceName{udisks::kInterfaceName},
                      sdbus::ObjectPath{udisks::kObjectPath}),
      options_{options} {
  for (auto managed_objects{GetManagedObjects()};
       const auto& [object_path, interfaces_and_properties] : managed_objects) {
    onInterfacesAdded(object_path, interfaces_and_properties);
  }

  spdlog::info("Scanned drives");

  registerProxy();
}

namespace {

template <class UdisksInterface>
bool HasInterface(InterfacesAndProperties& inp) {
  return inp.contains(sdbus::InterfaceName{UdisksInterface::INTERFACE_NAME});
}

}

void UdisksObjectManager::onInterfacesAdded(
    const sdbus::ObjectPath& object_path,
    InterfacesAndProperties interfaces_and_properties) {
  spdlog::debug("New object: {}", object_path.c_str());

  if (!HasInterface<udisks_sd::proxy_wrappers::UdisksBlock>(
          interfaces_and_properties)) {
    return;
  }

  auto block{std::make_unique<udisks_sd::proxy_wrappers::UdisksBlock>(
      getProxy().getConnection(), object_path)};

  std::unique_ptr<udisks_sd::proxy_wrappers::UdisksFilesystem> filesystem{};
  if (HasInterface<udisks_sd::proxy_wrappers::UdisksFilesystem>(
          interfaces_and_properties)) {
    filesystem = std::make_unique<udisks_sd::proxy_wrappers::UdisksFilesystem>(
        getProxy().getConnection(), object_path);
  }

  std::unique_ptr<udisks_sd::proxy_wrappers::UdisksLoop> loop{};
  if (HasInterface<udisks_sd::proxy_wrappers::UdisksLoop>(
          interfaces_and_properties)) {
    loop = std::make_unique<udisks_sd::proxy_wrappers::UdisksLoop>(
        getProxy().getConnection(), object_path);
  }

  std::unique_ptr<udisks_sd::proxy_wrappers::UdisksPartition> partition{};
  if (HasInterface<udisks_sd::proxy_wrappers::UdisksPartition>(
          interfaces_and_properties)) {
    partition = std::make_unique<udisks_sd::proxy_wrappers::UdisksPartition>(
        getProxy().getConnection(), object_path);
  }

  // Only block must be non-null. The rest can be null. The Drive member will be
  // automatically constructed if it exists.
  objects::BlockDevice blk_device{std::move(block), std::move(filesystem),
                                  std::move(loop), std::move(partition)};

  mount::TryAutomount(blk_device);

  spdlog::debug("Processed block device at {}", object_path.c_str());
}

void UdisksObjectManager::onInterfacesRemoved(
    [[maybe_unused]] const sdbus::ObjectPath& object_path,
    [[maybe_unused]] const std::vector<sdbus::InterfaceName>& interfaces) {}

}  // namespace managers
