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

/// UDisks objects, interface proxies, managers and related functions.
/// Contains the entrypoints to most of UDISKEN's logic, such as automounting.

#ifndef UDISKEN_UDISKS_HPP_
#define UDISKEN_UDISKS_HPP_

#include "options.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>
#include <udisks-sdbus-c++/udisks_proxy_wrappers.hpp>

#include <map>
#include <memory>
#include <vector>

namespace udisks {

constexpr auto kInterfaceName{"org.freedesktop.UDisks2"};
constexpr auto kObjectPath{"/org/freedesktop/UDisks2"};
static const sdbus::ServiceName kServiceName{kInterfaceName};

static const sdbus::ObjectPath kEmptyObjectPath{"/"};

}  // namespace udisks

namespace objects {

/// Drive object, which is the physical device behind its block device
/// objects.
class Drive {
 public:
  /// Construct a Drive object with the Drive interface proxy.
  ///
  /// @param drive Pointer to the drive interface for this object. Must be
  /// non-null.
  Drive(std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksDrive> drive);

  const sdbus::ObjectPath& ObjectPath() const;

  /// Get the drive interface proxy.
  ///
  /// @return Reference to the drive interface proxy, not the pointer.
  auto GetDrive() -> org::freedesktop::UDisks2::proxies::UdisksDrive&;

 private:
  /// Corresponding Drive interface for this block device.
  std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksDrive> drive_;
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
      std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksBlock> block,
      std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksFilesystem>
          filesystem = nullptr,
      std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksLoop> loop =
          nullptr,
      std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksPartition>
          partition = nullptr);

  const sdbus::ObjectPath& ObjectPath() const;

  /// Get the block interface proxy; this proxy always exists as long as
  /// the block device is valid.
  ///
  /// @return Reference to the block interface proxy, not the pointer.
  auto Block() -> org::freedesktop::UDisks2::proxies::UdisksBlock& {
    return *block_;
  }

  /// Get the filesystem interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @return Reference to the filesystem interface proxy, not the pointer.
  auto Filesystem() -> org::freedesktop::UDisks2::proxies::UdisksFilesystem&;
  bool HasFilesystem() { return filesystem_ != nullptr; }

  /// Get the loop device interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @return Reference to the loop device interface proxy, not the pointer.
  auto Loop() -> org::freedesktop::UDisks2::proxies::UdisksLoop&;
  bool HasLoop() { return loop_ != nullptr; }

  /// Get the partition interface proxy.
  ///
  /// @throws logic_error Tried to access an interface that is not implemented
  /// by the object.
  ///
  /// @return Reference to the partition interface proxy, not the pointer.
  auto Partition() -> org::freedesktop::UDisks2::proxies::UdisksPartition&;
  bool HasPartition() { return partition_ != nullptr; }

 private:
  /// Corresponding drive object for this block device. If it exists, it is
  /// automatically created.
  std::unique_ptr<Drive> drive_ = nullptr;
  /// Proxy to the block interface of this block device object.
  std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksBlock> block_;
  /// Proxy to the filesystem present on the block device.
  std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksFilesystem>
      filesystem_ = nullptr;
  /// Proxy to the loop device on the block device.
  std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksLoop> loop_ =
      nullptr;
  /// Proxy to the partition on the block device.
  std::unique_ptr<org::freedesktop::UDisks2::proxies::UdisksPartition>
      partition_ = nullptr;
};

}  // namespace objects

/// Entrypoints of most of UDISKEN's logic.
namespace managers {

/// UDisks top-level manager singleton object.
/// Exposes some meta information about UDisks, such as version.
class UdisksManager final
    : public sdbus::ProxyInterfaces<org::freedesktop::UDisks2::Manager_proxy> {
 public:
  /// Connect to UDisks using a system bus connection.
  ///
  /// @param connection System bus connection.
  explicit UdisksManager(sdbus::IConnection& connection);

  UdisksManager(const UdisksManager&) = delete;
  UdisksManager(UdisksManager&&) = delete;
  UdisksManager& operator=(const UdisksManager&) = delete;
  UdisksManager& operator=(UdisksManager&&) = delete;

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

  UdisksObjectManager(const UdisksObjectManager&) = delete;
  UdisksObjectManager(UdisksObjectManager&&) = delete;
  UdisksObjectManager& operator=(const UdisksObjectManager&) = delete;
  UdisksObjectManager& operator=(UdisksObjectManager&&) = delete;

  ~UdisksObjectManager() noexcept { unregisterProxy(); }

 private:
  /// Processes interfaces and the objects implementing them, and runs vital
  /// functions on these, such as automounting.
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
