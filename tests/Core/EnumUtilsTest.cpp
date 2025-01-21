#include "../Utils.hpp"

#include <Lux/Core/EnumUtils.hpp>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

enum class Enum
{
	A,
	B,
	Max_ = B
};

enum class EnumMaxOutside
{
	A,
	B
};

LUX_SET_ENUM_MAX(EnumMaxOutside, B);

TEST_CASE("EnumUtils", "[Core]")
{
	SECTION("only works with maxed enums")
	{
		enum class Invalid { A };

		[]<typename T> {
			CHECK_DOES_NOT_COMPILE(lux::EnumMax<T>);
			CHECK_DOES_NOT_COMPILE(lux::EnumCount<T>);
		}.operator()<Invalid>();
	}

	SECTION("max")
	{
		STATIC_CHECK(lux::EnumMax<Enum> == Enum::Max_);
		STATIC_CHECK(lux::EnumMax<EnumMaxOutside> == EnumMaxOutside::B);
	}

	SECTION("count")
	{
		STATIC_CHECK(lux::EnumCount<Enum> == 2);
		STATIC_CHECK(std::is_same_v<decltype(lux::EnumCount<Enum>), std::size_t const>);
	}
}
