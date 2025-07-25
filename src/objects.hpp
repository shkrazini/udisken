// UDISKEN: Linux drive automounter for the impatient.
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

#ifndef UDISKEN_OBJECTS_HPP_
#define UDISKEN_OBJECTS_HPP_

#include "interfaces.hpp"

#include <sdbus-c++/Types.h>

#include <memory>

namespace objects {

/// Block device object, upon which most UDISKEN actions take effect.
class BlockDevice {
 public:
  /// Create a Block device that will take ownership of the unique_ptrs to
  /// the proxy interfaces.
  ///
  /// Some automatic actions, such as automounting, will be executed.
  ///
  /// Unique_ptrs passed to this constructor will be moved to!
  BlockDevice(const sdbus::ObjectPath& object_path,
              std::unique_ptr<interfaces::UdisksBlock> block,
              std::unique_ptr<interfaces::UdisksFilesystem> filesystem);

 private:
  /// Object path, as found in /org/freedesktop/UDisks2/block_devices/.
  sdbus::ObjectPath object_path_;
  /// Corresponding Drive interface for this block device.
  // std::unique_ptr<UdisksDrive> drive;
  /// Proxy to the Block interface of this block device object.
  std::unique_ptr<interfaces::UdisksBlock> block_;
  /// Proxy to the Filesystem present on the block device.
  std::unique_ptr<interfaces::UdisksFilesystem> filesystem_;
};

#endif  // UDISKEN_OBJECTS_HPP_

}  // namespace objects
