// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include "uuidxx/uuid.h"

#include <cinttypes>
#include <cstdio>

namespace {

constexpr size_t k_canonical_len = 36;

}   // namespace

namespace uuidxx {

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
