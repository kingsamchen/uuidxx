
#include "catch2/catch.hpp"

#include "uuidxx/uuidxx.h"

#include <algorithm>
#include <string>
#include <vector>

namespace {

}   // namespace

namespace uuid {

TEST_CASE("Format compliant", "[v4]")
{
    auto uuid = make_v4();

    REQUIRE(uuid.version() == version::v4);

    auto s = uuid.to_string();
    REQUIRE(s.size() == 36);
    REQUIRE(s[14] == '4');
}

TEST_CASE("Uniquess", "[v4]")
{
    std::vector<std::string> ids;
    for (int i = 0; i < 10; ++i) {
        ids.push_back(make_v4().to_string());
    }

    std::sort(ids.begin(), ids.end());

    auto it = std::adjacent_find(ids.begin(), ids.end());
    REQUIRE(it == ids.end());
}

}   // namespace uuid
