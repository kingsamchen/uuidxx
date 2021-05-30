// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_UUID_H_
#define UUIDXX_UUID_H_

#include <array>
#include <cstdint>
#include <exception>
#include <string>
#include <string_view>

#include "uuidxx/clock_sequence.h"
#include "uuidxx/dce_host_identifier.h"
#include "uuidxx/node_fetcher.h"

namespace uuidxx {
namespace details {

struct gen_v1_t {
    explicit gen_v1_t() = default;
};

struct gen_v2_t {
    explicit gen_v2_t() = default;
};

struct gen_v3_t {
    explicit gen_v3_t() = default;
};

struct gen_v4_t {
    explicit gen_v4_t() = default;
};

struct gen_v5_t {
    explicit gen_v5_t() = default;
};

struct gen_from_str_t {
    explicit gen_from_str_t() = default;
};

struct gen_from_data_bytes_t {
    explicit gen_from_data_bytes_t() = default;
};

inline constexpr gen_v1_t gen_v1{};
inline constexpr gen_v2_t gen_v2{};
inline constexpr gen_v3_t gen_v3{};
inline constexpr gen_v4_t gen_v4{};
inline constexpr gen_v5_t gen_v5{};
inline constexpr gen_from_str_t gen_from_str{};
inline constexpr gen_from_data_bytes_t gen_from_data_bytes{};

} // namespace details

namespace version {

inline constexpr uint8_t v1 = 1u;
inline constexpr uint8_t v2 = 2u;
inline constexpr uint8_t v3 = 3u;
inline constexpr uint8_t v4 = 4u;
inline constexpr uint8_t v5 = 5u;

} // namespace version

class bad_uuid_string : public std::invalid_argument {
public:
    explicit bad_uuid_string(std::string_view uuid_str)
        : invalid_argument(gen_err_msg(uuid_str)) {}

    static std::string gen_err_msg(std::string_view uuid_str) {
        std::string msg("invalid uuid string: ");
        msg.append(uuid_str);
        return msg;
    }
};

struct data_bytes {
    uint32_t field1;
    uint16_t field2;
    uint16_t field3;
    uint8_t field4[8];

    constexpr data_bytes()
        : field1{},
          field2{},
          field3{},
          field4{} {}

    constexpr data_bytes(uint32_t f1, uint16_t f2, uint16_t f3, uint8_t f41, uint8_t f42,
                         uint8_t f43, uint8_t f44, uint8_t f45, uint8_t f46, uint8_t f47,
                         uint8_t f48)
        : field1(f1),
          field2(f2),
          field3(f3),
          field4{f41, f42, f43, f44, f45, f46, f47, f48} {}
};

class uuid {
public:
    // octets:  8 - 4 - 4 - 4 - 12
    //         -----------+--------
    // storage: data[0]     data[1]
    using data = std::array<uint64_t, 2>;

    static_assert(sizeof(data) == 16);
    static_assert(alignof(data) == 8);

public:
    template<typename NodeFetcher>
    uuid(NodeFetcher&& fetch, details::gen_v1_t)
        : data_{} {
        static_assert(valid_fetcher_t<NodeFetcher>::value);

        auto [ts, seq] = clock_sequence::instance().read();

        node_id id;
        fetch(id);

        data_[0] |= ts << 32;
        data_[0] |= (ts & UINT64_C(0x0000'ffff'0000'0000)) >> 16;
        data_[0] |= ts >> 48;

        data_[1] |= static_cast<uint64_t>(seq & 0xff00) << 48;
        data_[1] |= static_cast<uint64_t>(seq & 0xff) << 48;

        // Reversely copy into 0 ~ 47 bits of data_[1].
        auto ptr = reinterpret_cast<uint8_t*>(&data_[1]);
        for (auto it = id.rbegin(); it != id.rend();) {
            *ptr++ = *it++;
        }

        set_variant();
        set_version(version::v1);
    }

    uuid(host_id host, details::gen_v2_t);

    uuid(const uuid& ns, std::string_view name, details::gen_v3_t);

    template<typename RandGen>
    uuid(RandGen&& gen, details::gen_v4_t) {
        static_assert(std::is_same_v<uint64_t, decltype(gen())>);

        data_[0] = gen();
        data_[1] = gen();

        set_variant();
        set_version(version::v4);
    }

    uuid(const uuid& ns, std::string_view name, details::gen_v5_t);

    uuid(std::string_view src, details::gen_from_str_t);

    constexpr uuid(const data_bytes& bytes, details::gen_from_data_bytes_t)
        : data_{} {
        data_[0] |= static_cast<uint64_t>(bytes.field1) << 32;
        data_[0] |= static_cast<uint64_t>(bytes.field2) << 16;
        data_[0] |= static_cast<uint64_t>(bytes.field3);

        for (int i = 0; i < 8; ++i) {
            data_[1] |= static_cast<uint64_t>(bytes.field4[i]) << ((7 - i) * 8);
        }
    }

    uuid(const uuid&) = default;

    uuid(uuid&&) = default;

    uuid& operator=(const uuid&) = default;

    uuid& operator=(uuid&&) = default;

    ~uuid() = default;

    [[nodiscard]] uint8_t version() const noexcept {
        return static_cast<uint8_t>((data_[0] >> 12) & 0x0f);
    }

    [[nodiscard]] std::string to_string() const;

    // The value is implementation defined.
    const data& raw_data() const noexcept {
        return data_;
    }

private:
    // The RFC 4122 has only one variant.
    void set_variant() noexcept {
        data_[1] &= UINT64_C(0x3fff'ffff'ffff'ffff);
        data_[1] |= UINT64_C(0x8000'0000'0000'0000);
    }

    void set_version(uint8_t ver) noexcept {
        auto ver_bits = static_cast<uint64_t>(ver) << 12;
        data_[0] &= UINT64_C(0xffff'ffff'ffff'0fff);
        data_[0] |= ver_bits;
    }

private:
    data data_;
};

inline bool operator==(const uuid& lhs, const uuid& rhs) {
    return lhs.raw_data() == rhs.raw_data();
}

inline bool operator!=(const uuid& lhs, const uuid& rhs) {
    return !(lhs == rhs);
}

} // namespace uuidxx

#endif // UUIDXX_UUID_H_
