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
#include <udisks-sdbus-c++/udisks_errors.hpp>

#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

namespace udisks_api = org::freedesktop::UDisks2;

namespace objects {

Drive::Drive(std::unique_ptr<udisks_api::proxies::UdisksDrive> drive)
    : drive_{std::move(drive)} {
  if (!drive_) {
    throw std::invalid_argument("drive pointer must not be null");
  }
}

const sdbus::ObjectPath& Drive::ObjectPath() const {
  return drive_->getProxy().getObjectPath();
}

auto Drive::GetDrive() -> udisks_api::proxies::UdisksDrive& { return *drive_; }

BlockDevice::BlockDevice(
    std::unique_ptr<udisks_api::proxies::UdisksBlock> block,
    std::unique_ptr<udisks_api::proxies::UdisksFilesystem> filesystem,
    std::unique_ptr<udisks_api::proxies::UdisksLoop> loop,
    std::unique_ptr<udisks_api::proxies::UdisksPartition> partition)
    : block_{std::move(block)},
      filesystem_{std::move(filesystem)},
      loop_{std::move(loop)},
      partition_{std::move(partition)} {
  if (!block_) {
    throw std::invalid_argument("block pointer must not be null");
  }

  if (block_->Drive() != udisks::kEmptyObjectPath) {
    drive_ = std::make_unique<Drive>(
        std::make_unique<udisks_api::proxies::UdisksDrive>(
            block_->getProxy().getConnection(), block_->Drive()));
  }
}

const sdbus::ObjectPath& BlockDevice::ObjectPath() const {
  return block_->getProxy().getObjectPath();
}

auto BlockDevice::Filesystem() -> udisks_api::proxies::UdisksFilesystem& {
  if (!HasFilesystem()) {
    throw std::logic_error("object does not implement interface");
  }

  return *filesystem_;
}

auto BlockDevice::Loop() -> udisks_api::proxies::UdisksLoop& {
  if (!HasLoop()) {
    throw std::logic_error("object does not implement interface");
  }

  return *loop_;
}

auto BlockDevice::Partition() -> udisks_api::proxies::UdisksPartition& {
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

  if (!HasInterface<udisks_api::proxies::UdisksBlock>(
          interfaces_and_properties)) {
    return;
  }

  auto block{std::make_unique<udisks_api::proxies::UdisksBlock>(
      getProxy().getConnection(), object_path)};

  std::unique_ptr<udisks_api::proxies::UdisksFilesystem> filesystem{};
  if (HasInterface<udisks_api::proxies::UdisksFilesystem>(
          interfaces_and_properties)) {
    filesystem = std::make_unique<udisks_api::proxies::UdisksFilesystem>(
        getProxy().getConnection(), object_path);
  }

  std::unique_ptr<udisks_api::proxies::UdisksLoop> loop{};
  if (HasInterface<udisks_api::proxies::UdisksLoop>(
          interfaces_and_properties)) {
    loop = std::make_unique<udisks_api::proxies::UdisksLoop>(
        getProxy().getConnection(), object_path);
  }

  std::unique_ptr<udisks_api::proxies::UdisksPartition> partition{};
  if (HasInterface<udisks_api::proxies::UdisksPartition>(
          interfaces_and_properties)) {
    partition = std::make_unique<udisks_api::proxies::UdisksPartition>(
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
