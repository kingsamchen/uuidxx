
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

}   // namespace uuidxx
