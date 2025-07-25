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

/// Concrete objects implementing many UDisks interfaces.

#include "objects.hpp"

#include "proxies.hpp"

#include <sdbus-c++/Types.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>

namespace objects {

BlockDevice::BlockDevice(const sdbus::ObjectPath& object_path,
                         std::unique_ptr<proxies::UdisksBlock> block,
                         std::unique_ptr<proxies::UdisksFilesystem> filesystem)
    : object_path_{object_path},
      block_{std::move(block)},
      filesystem_{std::move(filesystem)} {
  if (block_->HintAuto()) {
    filesystem->Automount();
  }

  spdlog::debug("Got hint to not automount {}", object_path_.c_str());
}

}  // namespace objects
