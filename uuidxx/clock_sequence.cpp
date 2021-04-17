// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include "uuidxx/clock_sequence.h"

#include <random>

namespace uuid {

clock_sequence::clock_sequence()
    : last_time_(0),
      seq_(static_cast<uint16_t>(std::random_device{}()))
{}

// static
clock_sequence& clock_sequence::instance()
{
    static clock_sequence instance;
    return instance;
}

// static
uint64_t clock_sequence::get_timestamp_since_epoch()
{
    auto ts = k_epoch_diff + std::chrono::duration_cast<clock_sequence::duration>(
        std::chrono::system_clock::now().time_since_epoch());
    return ts.count();
}

std::tuple<uint64_t, uint16_t> clock_sequence::read()
{
    uint64_t now = get_timestamp_since_epoch();
    uint16_t seq;

    {
        std::lock_guard lock(mtx_);

        // The clock is set backward or read too fast.
        if (now <= last_time_) {
            ++seq_;
        }

        last_time_ = now;
        seq = seq_;
    }

    return std::make_tuple(now, seq);
}

}   // namespace uuid
