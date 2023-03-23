// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include "uuidxx/uuid.h"

#include <charconv>
#include <cinttypes>
#include <cstdio>
#include <cstring>

extern "C" {
#include "hash/md5.h"
#include "hash/sha1.h"
}

#include "uuidxx/endian_utils.h"

namespace uuidxx {
namespace {

constexpr size_t k_canonical_len = 36;

void md5_hash(const uuid::data& ns_data, std::string_view name, uuid::data& hashed_data) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, ns_data.data(), sizeof(ns_data));
    // NOLINTNEXTLINE(google-runtime-int)
    MD5_Update(&ctx, name.data(), static_cast<unsigned long>(name.size()));
    MD5_Final(reinterpret_cast<unsigned char*>(hashed_data.data()), &ctx);
}

void sha1_hash(const uuid::data& ns_data, std::string_view name, uuid::data& hashed_data) {
    uint8_t digest[20];

    SHA1_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(ns_data.data()), sizeof(ns_data));
    SHA1_Update(&ctx, reinterpret_cast<const uint8_t*>(name.data()), name.size());
    SHA1_Final(&ctx, digest);

    std::memcpy(hashed_data.data(), digest, sizeof(hashed_data));
}

template<typename Hash>
void hash_named_data_to_uuid_data(const uuid& ns, std::string_view name, uuid::data& out,
                                  Hash hash_sum) {
    uuid::data ns_data;
    ns_data[0] = byteswap(ns.raw_data()[0]);
    ns_data[1] = byteswap(ns.raw_data()[1]);

    hash_sum(ns_data, name, out);

    out[0] = byteswap(out[0]);
    out[1] = byteswap(out[1]);
}

// Strip enclosing braces if possible and do quick format check.
std::string_view canonicalize_uuid_str(std::string_view input) {
    auto uuid_str = input;
    if (uuid_str.size() == k_canonical_len + 2) {
        if (uuid_str.front() != '{' || uuid_str.back() != '}') {
            throw bad_uuid_string(input);
        }

        uuid_str = uuid_str.substr(1, k_canonical_len);
    }

    if ((uuid_str.size() != k_canonical_len) ||
        (uuid_str[8] != '-' || uuid_str[13] != '-' ||
         uuid_str[18] != '-' || uuid_str[23] != '-')) {
        throw bad_uuid_string(input);
    }

    return uuid_str;
}

} // namespace

uuid::uuid(host_id host, details::gen_v2_t) {
    // Need high 32-bit of ts and high 8-bit of seq.
    auto [ts, seq] = clock_sequence::instance().read();

    // `local_domain` replaces low 8-bit of seq.
    // `local_id` replaces low 32-bit of ts.
    auto local_domain = host.domain();
    auto local_id = host.id();

    node_id node;
    read_mac_addr_as_node_id(node);

    data_[0] |= static_cast<uint64_t>(local_id) << 32;
    data_[0] |= (ts & UINT64_C(0x0000'ffff'0000'0000)) >> 16;
    data_[0] |= ts >> 48;

    data_[1] |= static_cast<uint64_t>(seq & 0xff00) << 48;
    data_[1] |= static_cast<uint64_t>(local_domain) << 48;

    // Reversely copy into 0 ~ 47 bits of data_[1].
    auto ptr = reinterpret_cast<std::byte*>(&data_[1]);
    for (auto it = node.rbegin(); it != node.rend();) {
        *ptr++ = *it++;
    }

    set_variant();
    set_version(version::v2);
}

uuid::uuid(const uuid& ns, std::string_view name, details::gen_v3_t) {
    hash_named_data_to_uuid_data(ns, name, data_, md5_hash);

    set_variant();
    set_version(version::v3);
}

uuid::uuid(const uuid& ns, std::string_view name, details::gen_v5_t) {
    hash_named_data_to_uuid_data(ns, name, data_, sha1_hash);

    set_variant();
    set_version(version::v5);
}

uuid::uuid(std::string_view src, details::gen_from_str_t) {
    auto uuid_str = canonicalize_uuid_str(src);

    auto cvt = [uuid_str](size_t first, size_t last) -> uint64_t {
        auto begin = uuid_str.data() + first;
        auto end = uuid_str.data() + last;

        uint64_t value{0};
        if (auto result = std::from_chars(begin, end, value, 16); result.ec != std::errc()) {
            throw bad_uuid_string(uuid_str);
        }

        return value;
    };

    std::array<uint64_t, 5> parts{cvt(0, 8), cvt(9, 13), cvt(14, 18), cvt(19, 23), cvt(24, 36)};

    data_[0] |= parts[0] << 32;
    data_[0] |= parts[1] << 16;
    data_[0] |= parts[2];

    data_[1] |= parts[3] << 48;
    data_[1] |= parts[4];
}

std::string uuid::to_string() const {
    std::string s(k_canonical_len + 1, 0);
    constexpr char fmt[] = "%08" PRIx64 "-%04" PRIx64 "-%04" PRIx64 "-%04" PRIx64 "-%012" PRIx64;
    std::snprintf(s.data(), s.size(), fmt,
                  data_[0] >> 32, (data_[0] >> 16) & 0xffff, data_[0] & 0xffff,
                  data_[1] >> 48, data_[1] & UINT64_C(0xffff'ffff'ffff));
    s.resize(k_canonical_len);
    return s;
}

} // namespace uuidxx
