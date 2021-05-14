// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_DCE_HOST_IDENTIFIER_H_
#define UUIDXX_DCE_HOST_IDENTIFIER_H_

#include <cstdint>

#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#endif

namespace uuidxx {
namespace details {

// These two functions return -1 on non-POSIX systems, like on Windows.

inline uint32_t get_uid()
{
#if defined(_WIN32) || defined(_WIN64)
    return static_cast<uint32_t>(-1);
#else
    static_assert(std::is_same_v<uint32_t, uid_t>);
    return getuid();
#endif
}

inline uint32_t get_gid()
{
#if defined(_WIN32) || defined(_WIN64)
    return static_cast<uint32_t>(-1);
#else
    static_assert(std::is_same_v<uint32_t, uid_t>);
    return getgid();
#endif
}

}   // namespace details

enum class local_domain {
    person,
    group,
    org
};

class host_id {
public:
    host_id(local_domain domain, uint32_t id)
        : domain_(domain),
          id_(id)
    {}

    local_domain domain() const noexcept
    {
        return domain_;
    }

    uint32_t id() const noexcept
    {
        return id_;
    }

private:
    local_domain domain_;
    uint32_t id_;
};

inline host_id make_person_host()
{
    return host_id(local_domain::person, details::get_uid());
}

inline host_id make_group_host()
{
    return host_id(local_domain::group, details::get_gid());
}

}   // namespace uuidxx

#endif  // UUIDXX_DCE_HOST_IDENTIFIER_H_
