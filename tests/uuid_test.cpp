
#include "catch2/catch.hpp"

#include "uuidxx/uuidxx.h"

#include <random>

namespace {

struct rnd {
    std::mt19937_64 engine{std::random_device{}()};
    uint64_t operator()()
    {
        return engine();
    }
};

}   // namespace

namespace uuid {

TEST_CASE("Generation of uuid v4", "[v4]")
{
    auto uuid = make_v4(rnd{});

    REQUIRE(uuid.version() == version::v4);

    auto s = uuid.to_string();
    REQUIRE(s.size() == 36);
    REQUIRE(s[14] == '4');
}

}   // namespace uuid
