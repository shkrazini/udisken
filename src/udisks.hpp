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

/// Udisks objects, interface proxies, managers and related functions.
/// Contains the entrypoints to most of UDISKEN's logic, such as automounting.

#ifndef UDISKEN_UDISKS_HPP_
#define UDISKEN_UDISKS_HPP_

#include "options.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy.hpp>

#include <map>
#include <memory>
#include <vector>

namespace udisks_api = org::freedesktop::UDisks2;

namespace udisks {

constexpr auto kInterfaceName = "org.freedesktop.UDisks2";
constexpr auto kObjectPath = "/org/freedesktop/UDisks2";
static const sdbus::ServiceName kServiceName{kInterfaceName};

static const sdbus::ObjectPath kEmptyObjectPath{"/"};

}  // namespace udisks

namespace interfaces {

/// Proxy to a UDisks block interface.
///
/// UdisksBlockDevice always implements this interface.
class UdisksBlock final
    : public sdbus::ProxyInterfaces<udisks_api::Block_proxy> {
 public:
  /// Construct a block proxy.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks block interface.
  UdisksBlock(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksBlock(UdisksBlock&&) = delete;
  UdisksBlock(const UdisksBlock&) = delete;
  auto operator=(UdisksBlock&&) -> UdisksBlock& = delete;
  auto operator=(const UdisksBlock&) -> UdisksBlock& = delete;

  ~UdisksBlock() noexcept { unregisterProxy(); };
};

/// Proxy to a UDisks disk drive interface.
///
/// UdisksBlock might have an associated drive object and interface, if the
/// block device has an underlying disk drive (e.g., it is not a loop device).
class UdisksDrive final
    : public sdbus::ProxyInterfaces<udisks_api::Drive_proxy> {
 public:
  /// Construct a drive proxy.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks drive interface.
  UdisksDrive(sdbus::IConnection& connection,
              const sdbus::ObjectPath& object_path);

  UdisksDrive(const UdisksDrive&) = delete;
  UdisksDrive(UdisksDrive&&) = delete;
  auto operator=(const UdisksDrive&) -> UdisksDrive& = delete;
  auto operator=(UdisksDrive&&) -> UdisksDrive& = delete;

  ~UdisksDrive() noexcept { unregisterProxy(); }
};

/// Proxy to a UDisks mountable filesystem interface, contained in a
/// UdisksBlockDevice.
///
/// UdisksBlockDevice may implement this interface.
class UdisksFilesystem final
    : public sdbus::ProxyInterfaces<udisks_api::Filesystem_proxy> {
 public:
  /// Construct a filesystem proxy.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks Filesystem interface.
  UdisksFilesystem(sdbus::IConnection& connection,
                   const sdbus::ObjectPath& object_path);

  UdisksFilesystem(UdisksFilesystem&&) = delete;
  UdisksFilesystem(const UdisksFilesystem&) = delete;
  auto operator=(UdisksFilesystem&&) -> UdisksFilesystem& = delete;
  auto operator=(const UdisksFilesystem&) -> UdisksFilesystem& = delete;

  ~UdisksFilesystem() noexcept { unregisterProxy(); }
};

/// Proxy to a UDisks loop device interface.
///
/// UdisksBlockDevice may implement this interface.
class UdisksLoop : public sdbus::ProxyInterfaces<udisks_api::Loop_proxy> {
 public:
  /// Construct a loop proxy.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks loop interface.
  UdisksLoop(sdbus::IConnection& connection,
             const sdbus::ObjectPath& objectPath);

  UdisksLoop(UdisksLoop&&) = delete;
  UdisksLoop(const UdisksLoop&) = delete;
  auto operator=(UdisksLoop&&) -> UdisksLoop& = delete;
  auto operator=(const UdisksLoop&) -> UdisksLoop& = delete;

  ~UdisksLoop() noexcept { unregisterProxy(); }
};

/// Proxy to a UDisks partition interface.
///
/// UdisksBlockDevice may implement this interface.
class UdisksPartition
    : public sdbus::ProxyInterfaces<udisks_api::Partition_proxy> {
 public:
  /// Construct a partition proxy.
  ///
  /// @param connection System bus connection. Should be the same as used
  /// to construct the manager proxy (UdisksManager).
  /// @param object_path Object path to a UDisks partition interface.
  UdisksPartition(sdbus::IConnection& connection,
                  const sdbus::ObjectPath& objectPath);

  UdisksPartition(UdisksPartition&&) = delete;
  UdisksPartition(const UdisksPartition&) = delete;
  auto operator=(UdisksPartition&&) -> UdisksPartition& = delete;
  auto operator=(const UdisksPartition&) -> UdisksPartition& = delete;

  ~UdisksPartition() noexcept { unregisterProxy(); }
};

}  // namespace interfaces

namespace objects {

// This place is filled to the brim with unique_ptrs. Why? Because every
// sdbus-c++ interface proxies have their copy ctor/assignment operator deleted.
// So, there is not much other way.

/// Drive object, which is the physical device behind its block device
/// objects.
class Drive {
 public:
  /// Construct a Drive object with the Drive interface proxy.
  ///
  /// @param drive Pointer to the drive interface for this object. Must be
  /// non-null.
  Drive(std::unique_ptr<interfaces::UdisksDrive> drive);

  [[nodiscard]] auto ObjectPath() const -> const sdbus::ObjectPath&;

  /// Get the drive interface proxy.
  ///
  /// @returns Reference to the drive interface proxy, not the pointer.
  [[nodiscard]] auto drive() -> interfaces::UdisksDrive&;

 private:
  /// Corresponding Drive interface for this block device.
  std::unique_ptr<interfaces::UdisksDrive> drive_;
};

/// Block device object, upon which most UDISKEN actions take effect.
class BlockDevice {
 public:
  /// Create a Block device that will take ownership of the unique_ptrs to
  /// the proxy interfaces.
  ///
  /// The block interface is required to construct this device.
  /// All other interfaces are optional, and can take nullptr.
  ///
  /// The drive object will be made available automatically if it exists.
  ///
  /// Unique_ptrs passed to this constructor will be moved to!
  BlockDevice(
      std::unique_ptr<interfaces::UdisksBlock> block,
      std::unique_ptr<interfaces::UdisksFilesystem> filesystem = nullptr,
      std::unique_ptr<interfaces::UdisksLoop> loop = nullptr,
      std::unique_ptr<interfaces::UdisksPartition> partition = nullptr);

  [[nodiscard]] auto ObjectPath() const -> const sdbus::ObjectPath&;

  /// Get the block interface proxy; this proxy always exists as long as
  /// the block device is valid.
  ///
  /// @returns Reference to the block interface proxy, not the pointer.
  [[nodiscard]] auto block() -> interfaces::UdisksBlock& { return *block_; }

  /// Get the filesystem interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @returns Reference to the filesystem interface proxy, not the pointer.
  [[nodiscard]] auto filesystem() -> interfaces::UdisksFilesystem&;
  [[nodiscard]] auto HasFilesystem() -> bool { return filesystem_ != nullptr; }

  /// Get the loop device interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @returns Reference to the loop device interface proxy, not the pointer.
  [[nodiscard]] auto loop() -> interfaces::UdisksLoop&;
  [[nodiscard]] auto HasLoop() -> bool { return loop_ != nullptr; }

  /// Get the partition interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @returns Reference to the partition interface proxy, not the pointer.
  [[nodiscard]] auto partition() -> interfaces::UdisksPartition&;
  [[nodiscard]] auto HasPartition() -> bool { return partition_ != nullptr; }

 private:
  /// Corresponding drive object for this block device. If it exists, it is
  /// automatically created.
  std::unique_ptr<Drive> drive_ = nullptr;
  /// Proxy to the block interface of this block device object.
  std::unique_ptr<interfaces::UdisksBlock> block_;
  /// Proxy to the filesystem present on the block device.
  std::unique_ptr<interfaces::UdisksFilesystem> filesystem_ = nullptr;
  /// Proxy to the loop device on the block device.
  std::unique_ptr<interfaces::UdisksLoop> loop_ = nullptr;
  /// Proxy to the partition on the block device.
  std::unique_ptr<interfaces::UdisksPartition> partition_ = nullptr;
};

}  // namespace objects

/// Entrypoints of most of UDISKEN's logic.
namespace managers {

namespace udisks_api = org::freedesktop::UDisks2;

/// UDisks top-level manager singleton object.
/// Exposes some meta information about UDisks, such as version.
class UdisksManager final
    : public sdbus::ProxyInterfaces<udisks_api::Manager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksManager(sdbus::IConnection& connection);

  UdisksManager(const UdisksManager&) = delete;
  UdisksManager(UdisksManager&&) = delete;
  auto operator=(const UdisksManager&) -> UdisksManager& = delete;
  auto operator=(UdisksManager&&) -> UdisksManager& = delete;

  ~UdisksManager() noexcept { unregisterProxy(); }

  /// Object path to the manager singleton.
  static constexpr auto kObjectPath{"/org/freedesktop/UDisks2/Manager"};
};

using InterfacesAndProperties =
    const std::map<sdbus::InterfaceName,
                   std::map<sdbus::PropertyName, sdbus::Variant>>&;

/// Class handling UDisks objects and implemented interfaces.
/// Almost all UDISKEN actions are executed in this class' virtual functions.
class UdisksObjectManager final
    : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksObjectManager(sdbus::IConnection& connection,
                               options::Options options);

  UdisksObjectManager(UdisksObjectManager&&) = delete;
  UdisksObjectManager(const UdisksObjectManager&) = delete;
  auto operator=(UdisksObjectManager&&) -> UdisksObjectManager& = delete;
  auto operator=(const UdisksObjectManager&) -> UdisksObjectManager& = delete;

  ~UdisksObjectManager() noexcept { unregisterProxy(); }

 private:
  /// Houses all UDISKEN automatic actions (e.g., automounting).
  void onInterfacesAdded(
      const sdbus::ObjectPath& object_path,
      InterfacesAndProperties interfaces_and_properties) final;

  void onInterfacesRemoved(
      const sdbus::ObjectPath& object_path,
      const std::vector<sdbus::InterfaceName>& interfaces) final;

  options::Options options_;
};

}  // namespace managers

#endif  // UDISKEN_UDISKS_HPP_
