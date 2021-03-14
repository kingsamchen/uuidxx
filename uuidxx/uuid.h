// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_UUID_H_
#define UUIDXX_UUID_H_

#include <array>
#include <cstdint>
#include <string>

namespace uuid {
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

inline constexpr gen_v1_t gen_v1 {};
inline constexpr gen_v2_t gen_v2 {};
inline constexpr gen_v3_t gen_v3 {};
inline constexpr gen_v4_t gen_v4 {};
inline constexpr gen_v5_t gen_v5 {};

}   // namespace details

namespace version {

inline constexpr uint8_t v3 = 3u;
inline constexpr uint8_t v4 = 4u;
inline constexpr uint8_t v5 = 5u;

}   // namespace version

class uuid {
public:
    template<typename RandGen>
    uuid(RandGen&& gen, details::gen_v4_t)
    {
        static_assert(std::is_same_v<uint64_t, decltype(gen())>);

        data_[0] = gen();
        data_[1] = gen();

        set_variant();
        set_version(version::v4);
    }

    uuid(const uuid&) = default;

    uuid(uuid&&) = default;

    uuid& operator=(const uuid&) = default;

    uuid& operator=(uuid&&) = default;

    ~uuid() = default;

    [[nodiscard]] uint8_t version() const noexcept;

    [[nodiscard]] std::string to_string() const;

private:
    void set_variant() noexcept;

    void set_version(uint8_t ver) noexcept;

private:
    // octets:  8 - 4 - 4 - 4 - 12
    //         -----------+--------
    // storage: data[0]     data[1]
    using data = std::array<uint64_t, 2>;

    static_assert(sizeof(data) == 16);
    static_assert(alignof(data) == 8);

    data data_;
};

}   // namespace uuid

#endif  // UUIDXX_UUID_H_
