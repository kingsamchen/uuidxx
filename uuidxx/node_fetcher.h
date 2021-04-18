// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_NODE_FETCHER_H_
#define UUIDXX_NODE_FETCHER_H_

#include <array>
#include <cstdint>
#include <type_traits>

namespace uuid {

// A 48-bit device-related identifier.
using node_id = std::array<uint8_t, 6>;

template<typename Fetcher, typename=void>
struct valid_fetcher_t : std::false_type
{};

template<typename Fetcher>
struct valid_fetcher_t<Fetcher,
    std::void_t<decltype(std::decay_t<Fetcher>{}(std::declval<node_id&>()))>> : std::true_type
{};

void read_mac_addr_as_node_id(node_id& id);

using mac_addr_reader_t = decltype(read_mac_addr_as_node_id);

}   // namespace uuid

#endif  // UUIDXX_NODE_FETCHER_H_