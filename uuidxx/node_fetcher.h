// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_NODE_FETCHER_H_
#define UUIDXX_NODE_FETCHER_H_

#include <array>
#include <cstdint>

namespace uuid {

// A 48-bit device-related identifier.
using node_id = std::array<uint8_t, 6>;

void read_mac_addr_as_node_id(node_id& id);

using mac_addr_reader_t = decltype(read_mac_addr_as_node_id);

}   // namespace uuid

#endif  // UUIDXX_NODE_FETCHER_H_