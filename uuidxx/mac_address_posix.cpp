// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include <cstring>

#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <net/if.h>

#include "uuidxx/node_fetcher.h"

namespace uuidxx {

sockaddr_ll* find_adapter(ifaddrs* start, size_t mac_addr_len) {
    for (auto addr = start; addr; addr = addr->ifa_next) {
        // Skip the loopback adapter.
        if (!addr->ifa_addr || addr->ifa_addr->sa_family != AF_PACKET ||
            (addr->ifa_flags & IFF_LOOPBACK)) {
            continue;
        }

        auto phy_sock = reinterpret_cast<sockaddr_ll*>(addr->ifa_addr);
        if (phy_sock->sll_halen >= mac_addr_len) {
            return phy_sock;
        }
    }

    return nullptr;
}

bool load_mac_addr_from_sys(node_id& mac_addr) {
    ifaddrs* addrs = nullptr;
    if (getifaddrs(&addrs) == -1) {
        return false;
    }

    auto ifad = find_adapter(addrs, mac_addr.size());
    if (ifad) {
        std::memcpy(mac_addr.data(), ifad->sll_addr, mac_addr.size());
    }

    freeifaddrs(addrs);

    return ifad != nullptr;
}

} // namespace uuidxx
