// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include "uuidxx/node_fetcher.h"

#include <cstring>
#include <functional>
#include <mutex>

#include "uuidxx/rand_generator.h"

namespace uuidxx {

bool load_mac_addr_from_sys(node_id& mac_addr);

void load_mac_address(node_id& addr) {
    if (load_mac_addr_from_sys(addr)) {
        return;
    }

    auto rand = details::global_random_generator::instance()();
    static_assert(sizeof(rand) >= sizeof(addr));
    std::memcpy(addr.data(), &rand, addr.size());

    // Recommended by RFC.
    addr[0] |= 0x01;
}

void read_mac_addr_as_node_id(node_id& id) {
    // We should load mac address only once in the whole lifetime.
    static node_id mac_addr;
    static std::once_flag once;
    std::call_once(once, load_mac_address, std::ref(mac_addr));

    std::memcpy(id.data(), mac_addr.data(), mac_addr.size());
}

} // namespace uuidxx
