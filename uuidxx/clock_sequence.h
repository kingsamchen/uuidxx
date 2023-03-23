// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_CLOCK_SEQUENCE_H_
#define UUIDXX_CLOCK_SEQUENCE_H_

#include <chrono>
#include <mutex>

namespace uuidxx {

class clock_sequence {
private:
    // 100ns intervals.
    using duration = std::chrono::duration<uint64_t, std::ratio<1, 10'000'000>>;

public:
    ~clock_sequence() = default;

    clock_sequence(const clock_sequence&) = delete;

    clock_sequence(clock_sequence&&) = delete;

    clock_sequence& operator=(const clock_sequence&) = delete;

    clock_sequence& operator=(clock_sequence&&) = delete;

    static clock_sequence& instance();

    std::tuple<uint64_t, uint16_t> read();

private:
    clock_sequence();

    // Gets 100ns interval count since UUID epoch.
    static uint64_t get_timestamp_since_epoch();

private:
    std::mutex mtx_;
    uint64_t last_time_{0};
    uint16_t seq_;

    // Difference in 100ns intervals between UUID epoch (1582/10/15 00:00:00) and Unix
    // epoch (1970/01/01 00:00:00)
    static constexpr duration k_epoch_diff{122192928000000000};
};

} // namespace uuidxx

#endif // UUIDXX_CLOCK_SEQUENCE_H_
