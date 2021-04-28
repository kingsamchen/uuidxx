
#include "catch2/catch.hpp"

#include "uuidxx/uuidxx.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace {

}   // namespace

namespace uuidxx {

TEST_CASE("V4 Format compliant", "[v4]")
{
    auto uuid = make_v4();

    REQUIRE(uuid.version() == version::v4);

    auto s = uuid.to_string();
    REQUIRE(s.size() == 36);
    REQUIRE(s[14] == '4');
}

TEST_CASE("V4 Uniqueness", "[v4]")
{
    std::vector<std::string> ids;
    for (int i = 0; i < 10; ++i) {
        ids.push_back(make_v4().to_string());
    }

    std::sort(ids.begin(), ids.end());

    auto it = std::adjacent_find(ids.begin(), ids.end());
    REQUIRE(it == ids.end());
}

TEST_CASE("V1 Format compliant", "[v1]")
{
    auto uuid = make_v1();

    REQUIRE(uuid.version() == version::v1);

    auto s = uuid.to_string();
    REQUIRE(s.size() == 36);
    REQUIRE(s[14] == '1');
}

TEST_CASE("V1 Uniqueness and validation", "[v1]")
{
    std::vector<std::string> ids;
    for (int i = 0; i < 100; ++i) {
        ids.push_back(make_v1().to_string());
    }

    // V1 should be incremented on a single machine.
    REQUIRE(std::is_sorted(ids.begin(), ids.end()));

    auto it = std::adjacent_find(ids.cbegin(), ids.cend());
    REQUIRE(it == ids.cend());

    SECTION("last 6-digits(node-id) should be the same")
    {
        std::vector<std::string> nodes;
        std::transform(ids.begin(), ids.end(), std::back_inserter(nodes),
                       [](const std::string& id) {
                           auto pos = id.rfind('-');
                           return id.substr(pos + 1, id.size() - pos);
                       });
        for (auto iter = std::next(nodes.begin()); iter != nodes.end(); ++iter) {
            REQUIRE(*iter == *std::prev(iter));
        }
    }

    SECTION("the combination of ts and clock-seq should be incrementing")
    {
        // <low-32-bit ts, clock-seq>
        using ts_pair = std::pair<unsigned long, unsigned long>;
        std::vector<ts_pair> tss;
        for (const auto& id : ids) {
            auto low_ts = "0x" + id.substr(0, 8);
            auto clock_seq = "0x" + id.substr(19, 4);
            tss.emplace_back(std::stoul(low_ts, nullptr, 16), std::stoul(clock_seq, nullptr, 16));
        }

        // If both the low 32-bit of ts are equal, they must differ in clock-sequence.
        for (auto iter = tss.begin() + 1; iter != tss.end(); ++iter) {
            auto prev_iter = iter - 1;
            REQUIRE(((iter->first > prev_iter->first) ||
                    (iter->first == prev_iter->first && iter->second > prev_iter->second)));
        }
    }
}

TEST_CASE("V2 generation and validation", "[v2]")
{
    std::vector<std::string> ids;
    for (int i = 0; i < 10; ++i) {
        ids.push_back(make_v2(make_person_host()).to_string());
    }

    SECTION("last 6-digits(node-id) should be the same")
    {
        std::vector<std::string> nodes;
        std::transform(ids.begin(), ids.end(), std::back_inserter(nodes),
                       [](const std::string& id) {
                           auto pos = id.rfind('-');
                           return id.substr(pos + 1, id.size() - pos);
                       });
        for (auto iter = std::next(nodes.begin()); iter != nodes.end(); ++iter) {
            REQUIRE(*iter == *std::prev(iter));
        }
    }

    SECTION("first 8-digits(local identifier) should be the same")
    {
        std::vector<std::string> local_ids;
        for (const auto& id : ids) {
            local_ids.push_back(id.substr(0, 8));
        }
        for (auto it = local_ids.begin() + 1; it != local_ids.end(); ++it) {
            REQUIRE(*it == *(it - 1));
        }
    }

    SECTION("non-decreasing part")
    {
        std::vector<std::pair<unsigned long, unsigned long>> tss;
        for (const auto& id : ids) {
            auto ts_mid = id.substr(9, 4);
            auto ts_high = id.substr(14, 4);
            auto ts = std::stoul(ts_high + ts_mid, nullptr, 16);
            auto seq = std::stoul(id.substr(19, 4), nullptr, 16);
            tss.emplace_back(ts, seq);
        }

        for (auto it = tss.begin() + 1; it != tss.end(); ++it) {
            auto prev_it = it - 1;
            REQUIRE (
                ((prev_it->first < it->first) ||
                (prev_it->first == it->first && prev_it->second <= it->second)));
        }
    }
}

TEST_CASE("V3 Generation", "[v3]")
{
    auto ns = make_from("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
    auto id = make_v3(ns, "www.widgets.com");
    REQUIRE(id.to_string() == "3d813cbb-47fb-32ba-91df-831e1593ac29");
}

TEST_CASE("Generate from string", "[from_str]")
{
    SECTION("from specific string")
    {
        std::string src_str("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
        auto uuid = make_from(src_str);
        CHECK(uuid.to_string() == src_str);
    }

    SECTION("from and to v4")
    {
        auto src_str = make_v4().to_string();
        auto uuid = make_from(src_str);
        CHECK(uuid.to_string() == src_str);
    }
}

}   // namespace uuidxx
