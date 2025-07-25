// udisken: an automounter that stays quite quiet.
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

#include "filesystem.hpp"

#include <sdbus-c++/Types.h>

#include <memory>

namespace proxies {

/// Block device object, upon which most udisken actions take effect.
struct BlockDevice {
  /// Object path, as found in /org/freedesktop/UDisks2/block_devices/.
  sdbus::ObjectPath object_path;
  /// Corresponding Drive interface for this block device.
  // std::unique_ptr<UdisksDrive> drive;
  /// Proxy to the Block interface of this block device object.
  std::unique_ptr<UdisksBlock> block;
  /// Proxy to the Filesystem present on the block device.
  std::unique_ptr<UdisksFilesystem> filesystem;
};

}  // namespace proxies
