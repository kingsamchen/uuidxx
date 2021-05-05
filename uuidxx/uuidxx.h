// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_UUIDXX_H_
#define UUIDXX_UUIDXX_H_

#include "uuidxx/dce_host_identifier.h"
#include "uuidxx/rand_generator.h"
#include "uuidxx/uuid.h"

namespace uuidxx {

template<typename NodeFetcher=mac_addr_reader_t>
uuid make_v1(NodeFetcher&& fetcher=read_mac_addr_as_node_id)
{
    return uuid(std::forward<NodeFetcher>(fetcher), details::gen_v1);
}

// v2 is provided for completeness only, DO NOT use it in production.
// This implementation would result in generating UUIDs that are not very unique.
// The spec, i.e. DCE 1.1, of v2 itself is quite vague on its implementation, and
// lack of specific explanatory demo code makes this implementation failed to meet
// the spec.
inline uuid make_v2(host_id host)
{
    return uuid(host, details::gen_v2);
}

inline uuid make_v3(const uuid& ns, std::string_view name)
{
    return uuid(ns, name, details::gen_v3);
}

template<typename RandGen=default_rand_gen_t>
uuid make_v4(RandGen&& gen=default_rand_gen)
{
    return uuid(std::forward<RandGen>(gen), details::gen_v4);
}

inline uuid make_v5(const uuid& ns, std::string_view name)
{
    return uuid(ns, name, details::gen_v5);
}

// Only format like `xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx` and
// `{xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx}` are supported.
// Would throw `bad_uuid_string` if `src` is not a valid uuid string.
inline uuid make_from(std::string_view src)
{
    return uuid(src, details::gen_from_str);
}

constexpr uuid make_from(const data_bytes& bytes)
{
    return uuid(bytes, details::gen_from_data_bytes);
}

const constexpr auto k_nil = make_from(data_bytes{});

// Predefined constants name space ids defined in RFC 4122

// 6ba7b810-9dad-11d1-80b4-00c04fd430c8
const constexpr auto k_namespace_dns = make_from(
    data_bytes{0x6ba7b810, 0x9dad, 0x11d1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

// 6ba7b811-9dad-11d1-80b4-00c04fd430c8
const constexpr auto k_namespace_url = make_from(
    data_bytes{0x6ba7b811, 0x9dad, 0x11d1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

// 6ba7b812-9dad-11d1-80b4-00c04fd430c8
const constexpr auto k_namespace_oid = make_from(
    data_bytes{0x6ba7b812, 0x9dad, 0x11d1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

// 6ba7b814-9dad-11d1-80b4-00c04fd430c8
const constexpr auto k_namespace_x500 = make_from(
    data_bytes{0x6ba7b814, 0x9dad, 0x11d1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8});

}   // namespace uuidxx

#endif  // UUIDXX_UUIDXX_H_
