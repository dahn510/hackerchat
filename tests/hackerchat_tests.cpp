#include <catch2/catch.hpp>
#include <hackerchat/version.hpp>

TEST_CASE("test version function")
{
	REQUIRE(hackerchat::get_version().compare("0.1.0") == 0);
}
