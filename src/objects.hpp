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
#include <stdexcept>
#include <string>

namespace objects {

/// Block device object, upon which most UDISKEN actions take effect.
class BlockDevice {
 public:
  /// Create a Block device that will take ownership of the unique_ptrs to
  /// the proxy interfaces.
  ///
  /// Unique_ptrs passed to this constructor will be moved to!
  BlockDevice(
      const sdbus::ObjectPath& object_path,
      std::unique_ptr<interfaces::UdisksBlock> block,
      std::unique_ptr<interfaces::UdisksFilesystem> filesystem = nullptr,
      std::unique_ptr<interfaces::UdisksLoop> loop = nullptr,
      std::unique_ptr<interfaces::UdisksLoop> partition = nullptr);

  [[nodiscard]] auto ObjectPath() const -> const sdbus::ObjectPath& {
    return object_path_;
  }

  /// @brief Get the block interface proxy; this proxy always exists as long as
  /// the block device is valid.
  ///
  /// @returns Reference to the block interface proxy, not the pointer.
  [[nodiscard]] auto Block() -> interfaces::UdisksBlock& { return *block_; }

  /// @brief Get the filesystem interface proxy.
  ///
  /// @throws InterfaceNotImplemented if trying to access a non-existent
  /// interface
  ///
  /// @returns Reference to the filesystem interface proxy, not the pointer.
  [[nodiscard]] auto Filesystem() -> interfaces::UdisksFilesystem& {
    if (!HasFilesystem()) {
      throw std::logic_error("interface does not exist");
    }

    return *filesystem_;
  }
  [[nodiscard]] auto HasFilesystem() -> bool { return filesystem_ != nullptr; }

  /// @brief Get the loop device interface proxy.
  ///
  /// @returns Reference to the loop device interface proxy, not the pointer.
  [[nodiscard]] auto Loop() -> interfaces::UdisksLoop& {
    if (!HasLoop()) {
      throw std::logic_error("interface does not exist");
    }
    return *loop_;
  }
  [[nodiscard]] auto HasLoop() -> bool { return loop_ != nullptr; }

  /// @brief Get the partition interface proxy.
  ///
  /// @returns Reference to the partition interface proxy, not the pointer.
  [[nodiscard]] auto Partition() -> interfaces::UdisksLoop& {
    if (!HasPartition()) {
      throw std::logic_error("interface does not exist");
    }

    return *partition_;
  }
  [[nodiscard]] auto HasPartition() -> bool { return partition_ != nullptr; }

 private:
  /// Object path, as found in /org/freedesktop/UDisks2/block_devices/.
  sdbus::ObjectPath object_path_;
  /// Corresponding Drive interface for this block device.
  // std::unique_ptr<UdisksDrive> drive;
  /// Proxy to the block interface of this block device object.
  std::unique_ptr<interfaces::UdisksBlock> block_;
  /// Proxy to the filesystem present on the block device.
  std::unique_ptr<interfaces::UdisksFilesystem> filesystem_;
  /// Proxy to the loop device on the block device.
  std::unique_ptr<interfaces::UdisksLoop> loop_;
  /// Proxy to the partition on the block device.
  std::unique_ptr<interfaces::UdisksLoop> partition_;
};

/// Automount, TODO(blackma9ick): if UDISKEN is configured to do so.
///
/// @return Path to mount point after mounting, or nothing if the
/// filesystem is already mounted somewhere.
///
/// @throws sdbus::Error Error returned by UDisks if automounting
/// failed. Does not throw if filesystem is already mounted somewhere.
auto TryAutomount(objects::BlockDevice& blk_device)
    -> std::optional<std::string>;

}  // namespace objects

#endif  // UDISKEN_OBJECTS_HPP_
