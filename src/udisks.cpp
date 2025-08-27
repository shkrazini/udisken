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

namespace interfaces {

UdisksBlock::UdisksBlock(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, sdbus::ServiceName{udisks::kInterfaceName},
                      object_path) {
  registerProxy();
}

UdisksDrive::UdisksDrive(sdbus::IConnection& connection,
                         const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, udisks::kServiceName, object_path) {
  registerProxy();
}

UdisksFilesystem::UdisksFilesystem(sdbus::IConnection& connection,
                                   const sdbus::ObjectPath& object_path)
    : ProxyInterfaces(connection, udisks::kServiceName, object_path) {
  registerProxy();
}

UdisksLoop::UdisksLoop(sdbus::IConnection& connection,
                       const sdbus::ObjectPath& objectPath)
    : ProxyInterfaces<Loop_proxy>(connection, udisks::kServiceName,
                                  objectPath) {
  registerProxy();
}

UdisksPartition::UdisksPartition(sdbus::IConnection& connection,
                                 const sdbus::ObjectPath& objectPath)
    : ProxyInterfaces<Partition_proxy>(connection, udisks::kServiceName,
                                       objectPath) {
  registerProxy();
}

}  // namespace interfaces

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

auto Drive::GetDrive() -> interfaces::UdisksDrive& { return *drive_; }

BlockDevice::BlockDevice(
    std::unique_ptr<interfaces::UdisksBlock> block,
    std::unique_ptr<interfaces::UdisksFilesystem> filesystem,
    std::unique_ptr<interfaces::UdisksLoop> loop,
    std::unique_ptr<interfaces::UdisksPartition> partition)
    : block_{std::move(block)},
      filesystem_{std::move(filesystem)},
      loop_{std::move(loop)},
      partition_{std::move(partition)} {
  if (!block_) {
    throw std::invalid_argument("block pointer must not be null");
  }

  if (block_->Drive() != udisks::kEmptyObjectPath) {
    drive_ = std::make_unique<Drive>(std::make_unique<interfaces::UdisksDrive>(
        block_->getProxy().getConnection(), block_->Drive()));
  }
}

auto BlockDevice::ObjectPath() const -> const sdbus::ObjectPath& {
  return block_->getProxy().getObjectPath();
}

auto BlockDevice::Filesystem() -> interfaces::UdisksFilesystem& {
  if (!HasFilesystem()) {
    throw std::logic_error("object does not implement interface");
  }

  return *filesystem_;
}

auto BlockDevice::Loop() -> interfaces::UdisksLoop& {
  if (!HasLoop()) {
    throw std::logic_error("object does not implement interface");
  }

  return *loop_;
}

auto BlockDevice::Partition() -> interfaces::UdisksPartition& {
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
auto HasInterface(InterfacesAndProperties& inp) -> bool {
  return inp.contains(sdbus::InterfaceName{UdisksInterface::INTERFACE_NAME});
}

}

void UdisksObjectManager::onInterfacesAdded(
    const sdbus::ObjectPath& object_path,
    InterfacesAndProperties interfaces_and_properties) {
  spdlog::debug("New object: {}", object_path.c_str());

  if (!HasInterface<interfaces::UdisksBlock>(interfaces_and_properties)) {
    return;
  }

  auto block{std::make_unique<interfaces::UdisksBlock>(
      getProxy().getConnection(), object_path)};

  std::unique_ptr<interfaces::UdisksFilesystem> filesystem{};
  if (HasInterface<interfaces::UdisksFilesystem>(interfaces_and_properties)) {
    filesystem = std::make_unique<interfaces::UdisksFilesystem>(
        getProxy().getConnection(), object_path);
  }

  std::unique_ptr<interfaces::UdisksLoop> loop{};
  if (HasInterface<interfaces::UdisksLoop>(interfaces_and_properties)) {
    loop = std::make_unique<interfaces::UdisksLoop>(getProxy().getConnection(),
                                                    object_path);
  }

  std::unique_ptr<interfaces::UdisksPartition> partition{};
  if (HasInterface<interfaces::UdisksPartition>(interfaces_and_properties)) {
    partition = std::make_unique<interfaces::UdisksPartition>(
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
