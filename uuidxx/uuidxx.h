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

template<typename RandGen=default_rand_gen_t>
uuid make_v4(RandGen&& gen=default_rand_gen)
{
    return uuid(std::forward<RandGen>(gen), details::gen_v4);
}

}   // namespace uuidxx

#endif  // UUIDXX_UUIDXX_H_
