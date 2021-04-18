// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_RAND_GENERATOR_H_
#define UUIDXX_RAND_GENERATOR_H_

#include <cstdint>
#include <mutex>
#include <random>

namespace uuidxx {

namespace details {

// MSVC's implementation of std::random_device is unfortunately cryptographically secure,
// and thus, it runs perceivably slower than pseudo-rand engines and may result in a
// blocking call.
// So we use std::random_device to seed our internal engine, only in its first use; and
// maintaining an internal engine requires explicit locking for thread-safety.
// This class is thread-safe.
class global_random_generator {
public:
    ~global_random_generator() = default;

    global_random_generator(const global_random_generator&) = delete;

    global_random_generator(global_random_generator&&) = delete;

    global_random_generator& operator=(const global_random_generator&) = delete;

    global_random_generator& operator=(global_random_generator&&) = delete;

    static global_random_generator& instance()
    {
        static global_random_generator instance;
        return instance;
    }

    uint64_t operator()()
    {
        std::lock_guard lock(mtx_);
        return engine_();
    }

private:
    global_random_generator()
        : engine_(std::random_device{}())
    {}

private:
    std::mutex mtx_;
    std::mt19937_64 engine_;
};

}   // namespace details

inline uint64_t default_rand_gen()
{
    return details::global_random_generator::instance()();
}

using default_rand_gen_t = decltype(default_rand_gen);

}   // namespace uuidxx

#endif  // UUIDXX_RAND_GENERATOR_H_
