// Copyright (c) 2021 Kingsley Chen <kingsamchen@gmail.com>
// This file is subject to the terms of license that can be
// found in the LICENSE file.

#include <memory>

#include <WinSock2.h>
#include <iphlpapi.h>

#include "node_fetcher.h"

#pragma comment(lib, "IPHLPAPI.lib")

namespace uuid {

bool load_mac_addr_from_sys(node_id& mac_addr)
{
    // Initial buffer size in 16-KB should be enough for most cases, since it's quite
    // expensive to call `GetAdaptersAddresses()`.
    // Also, we leave the `buf` uninitialized to save a few cycles.
    ULONG buf_len = 16 * 1024;
    std::unique_ptr<uint8_t[]> buf(new uint8_t[buf_len]);

    auto as_adapter_address = [](const std::unique_ptr<uint8_t[]>& raw) {
        return reinterpret_cast<IP_ADAPTER_ADDRESSES*>(raw.get());
    };

    auto rv = GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, as_adapter_address(buf), &buf_len);
    if (rv != ERROR_SUCCESS) {
        if (rv != ERROR_BUFFER_OVERFLOW) {
            return false;
        }

        // Enlarge and try again.
        buf.reset(new uint8_t[buf_len]);
        rv = GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, as_adapter_address(buf), &buf_len);
        if (rv != ERROR_SUCCESS) {
            return false;
        }
    }

    for (auto addr = as_adapter_address(buf); addr; addr = addr->Next) {
        if (addr->PhysicalAddressLength >= mac_addr.size()) {
            std::memcpy(mac_addr.data(), addr->PhysicalAddress, mac_addr.size());
            return true;
        }
    }

    return false;
}

}   // namespace uuid
