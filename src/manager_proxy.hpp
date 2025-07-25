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

#ifndef SRC_MANAGER_PROXY_HPP_
#define SRC_MANAGER_PROXY_HPP_

#include "udisks-proxy.hpp"

#include <sdbus-c++/IConnection.h>
#include <sdbus-c++/ProxyInterfaces.h>
#include <sdbus-c++/Types.h>

#include <utility>

class ManagerProxy final
    : public sdbus::ProxyInterfaces<org::freedesktop::UDisks2::Manager_proxy> {
 public:
  ManagerProxy(sdbus::IConnection& connection, sdbus::ServiceName destination,
               sdbus::ObjectPath object_path)
      : ProxyInterfaces(connection, std::move(destination),
                        std::move(object_path)) {
    registerProxy();
  }

  ManagerProxy() = delete;
  ManagerProxy(const ManagerProxy&) = delete;
  ManagerProxy(ManagerProxy&&) = delete;
  auto operator=(const ManagerProxy&) -> ManagerProxy& = delete;
  auto operator=(ManagerProxy&&) -> ManagerProxy& = delete;

  ~ManagerProxy() { unregisterProxy(); }
};

#endif  // SRC_MANAGER_PROXY_HPP_
