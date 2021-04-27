// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include "uuidxx/uuid.h"

#include <cinttypes>
#include <cstdio>

extern "C" {
#include "hash/md5.h"
}

#include "uuidxx/endian_utils.h"

namespace uuidxx {
namespace {

constexpr size_t k_canonical_len = 36;

void md5_hash(const uuid::data& ns_data, std::string_view name, uuid::data& hashed_data)
{
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, ns_data.data(), sizeof(ns_data));
    MD5_Update(&ctx, name.data(), static_cast<unsigned long>(name.size()));
    MD5_Final(reinterpret_cast<unsigned char*>(hashed_data.data()), &ctx);
}

template<typename Hash>
void hash_named_data_to_uuid_data(const uuid& ns, std::string_view name, uuid::data& out, Hash hash_sum)
{
    uuid::data ns_data;
    ns_data[0] = host_to_network(ns.raw_data()[0]);
    ns_data[1] = host_to_network(ns.raw_data()[1]);

    hash_sum(ns_data, name, out);

    out[0] = network_to_host(out[0]);
    out[1] = network_to_host(out[1]);
}

}   // namespace

uuid::uuid(const uuid& ns, std::string_view name, details::gen_v3_t)
{
    hash_named_data_to_uuid_data(ns, name, data_, md5_hash);

    set_variant();
    set_version(version::v3);
}

void uuid::set_variant() noexcept
{
    data_[1] &= UINT64_C(0x3fff'ffff'ffff'ffff);
    data_[1] |= UINT64_C(0x8000'0000'0000'0000);
}

void uuid::set_version(uint8_t ver) noexcept
{
    auto ver_bits = static_cast<uint64_t>(ver) << 12;
    data_[0] &= UINT64_C(0xffff'ffff'ffff'0fff);
    data_[0] |= ver_bits;
}

uint8_t uuid::version() const noexcept
{
    return static_cast<uint8_t>((data_[0] >> 12) & 0x0f);
}

std::string uuid::to_string() const
{
    std::string s(k_canonical_len + 1, 0);
    constexpr char fmt[] = "%08" PRIx64 "-%04" PRIx64 "-%04" PRIx64 "-%04" PRIx64 "-%012" PRIx64;
    std::snprintf(s.data(), s.size(), fmt,
                  data_[0] >> 32, (data_[0] >> 16) & 0xffff, data_[0] & 0xffff,
                  data_[1] >> 48, data_[1] & UINT64_C(0xffff'ffff'ffff));
    s.resize(k_canonical_len);
    return s;
}

}   // namespace uuidxx
