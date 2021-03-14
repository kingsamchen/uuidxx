// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#ifndef UUIDXX_UUIDXX_H_
#define UUIDXX_UUIDXX_H_

#include "uuidxx/uuid.h"

namespace uuid {

template<typename RandGen>
uuid make_v4(RandGen&& gen)
{
    return uuid(std::forward<RandGen>(gen), details::gen_v4);
}

}   // namespace uuid

#endif  // UUIDXX_UUIDXX_H_
