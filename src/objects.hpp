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

#ifndef UDISKEN_OBJECTS_HPP_
#define UDISKEN_OBJECTS_HPP_

#include "interfaces.hpp"

#include <sdbus-c++/Types.h>

#include <memory>
#include <string>

namespace objects {

// This place is filled to the brim with unique_ptrs. Why? Because every
// sdbus-c++ interface proxies have their copy ctor/assignment operator deleted.
// So, there is not much other way.

/// Drive object, which is the physical device behind its block device
/// objects.
// TODO(blackma9ick): directly take the UdisksDrive ctor parameters to construct
// it directly? Would be cleaner than requiring an interface: it creates a nest
// of make_uniques.
class Drive {
 public:
  /// Construct a Drive object with the Drive interface proxy.
  ///
  /// @param drive Pointer to the drive interface for this object. Must be
  /// non-null.
  // NOLINTNEXTLINE
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
  // NOLINTNEXTLINE
  BlockDevice(
      std::unique_ptr<interfaces::UdisksBlock> block,
      std::unique_ptr<interfaces::UdisksFilesystem> filesystem = nullptr,
      std::unique_ptr<interfaces::UdisksLoop> loop = nullptr,
      std::unique_ptr<interfaces::UdisksLoop> partition = nullptr);

  [[nodiscard]] auto ObjectPath() const -> const sdbus::ObjectPath&;

  /// Get the block interface proxy; this proxy always exists as long as
  /// the block device is valid.
  ///
  /// @returns Reference to the block interface proxy, not the pointer.
  [[nodiscard]] auto block() -> interfaces::UdisksBlock& { return *block_; }

  /// Get the filesystem interface proxy.
  ///
  /// @throws InterfaceNotImplemented if trying to access a non-existent
  /// interface
  ///
  /// @returns Reference to the filesystem interface proxy, not the pointer.
  [[nodiscard]] auto filesystem() -> interfaces::UdisksFilesystem&;
  [[nodiscard]] auto HasFilesystem() -> bool { return filesystem_ != nullptr; }

  /// Get the loop device interface proxy.
  ///
  /// @returns Reference to the loop device interface proxy, not the pointer.
  [[nodiscard]] auto loop() -> interfaces::UdisksLoop&;
  [[nodiscard]] auto HasLoop() -> bool { return loop_ != nullptr; }

  /// Get the partition interface proxy.
  ///
  /// @returns Reference to the partition interface proxy, not the pointer.
  [[nodiscard]] auto partition() -> interfaces::UdisksLoop&;
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
  std::unique_ptr<interfaces::UdisksLoop> partition_ = nullptr;
};

/// Automount.
///
/// @return Path to mount point after mounting, or nothing if the
/// filesystem is already mounted somewhere.
///
/// @throws sdbus::Error Error returned by UDisks if automounting
/// failed. Does not throw if filesystem is already mounted somewhere.
auto TryMount(objects::BlockDevice& blk_device) -> std::optional<std::string>;

}  // namespace objects

#endif  // UDISKEN_OBJECTS_HPP_
