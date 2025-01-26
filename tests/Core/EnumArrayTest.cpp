#include "../Utils.hpp"

#include <Lux/Core/EnumArray.hpp>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

enum class Enum
{
	A,
	B,
	Max_ = B
};

TEST_CASE("EnumArray", "[Core]")
{
	SECTION("only works with maxed enums")
	{
		enum class Invalid { A };

		[]<typename T> {
			CHECK_DOES_NOT_COMPILE((lux::EnumArray<T, int>()));
		}.operator()<Invalid>();
	}

	SECTION("ctor")
	{
		// default
		{
			auto const a = lux::EnumArray<Enum, int>();
			CHECK(a[Enum::A] == 0);
			CHECK(a[Enum::B] == 0);
		}

		// aggregate
		{
			auto const a = lux::EnumArray<Enum, int> { 1, 2 };
			CHECK(a[Enum::A] == 1);
			CHECK(a[Enum::B] == 2);
		}
	}

	SECTION("data")
	{
		// const
		{
			auto const a = lux::EnumArray<Enum, int> { 1, 2 };
			auto data = a.data();
			STATIC_CHECK(std::is_same_v<decltype(data), int const*>);
			CHECK(data[0] == 1);
			CHECK(data[1] == 2);
		}

		// non const
		{
			auto a = lux::EnumArray<Enum, int> { 1, 2 };
			auto data = a.data();
			data[0] = 3;
			data[1] = 4;
			CHECK(a[Enum::A] == 3);
			CHECK(a[Enum::B] == 4);
		}
	}

	SECTION("iteration (begin/end)")
	{
		// const
		{
			auto const a = lux::EnumArray<Enum, int> { 1, 2 };

			for (auto [e, v] : a)
			{
				STATIC_CHECK(std::is_same_v<decltype(v), int const&>);
				switch (e)
				{
					case Enum::A: CHECK(v == 1); break;
					case Enum::B: CHECK(v == 2); break;
				}
			}
		}

		// non const
		{
			auto a = lux::EnumArray<Enum, int> { 1, 2 };

			for (auto [e, v] : a)
			{
				switch (e)
				{
					case Enum::A: v = 3; break;
					case Enum::B: v = 4; break;
				}
			}

			CHECK(a[Enum::A] == 3);
			CHECK(a[Enum::B] == 4);
		}

		// non const with const value type
		{
			auto a = lux::EnumArray<Enum, int const> { 1, 2 };

			for (auto [e, v] : a)
			{
				STATIC_CHECK(std::is_same_v<decltype(v), int const&>);
			}
		}

		// const with const value type
		{
			auto const a = lux::EnumArray<Enum, int const> { 1, 2 };

			for (auto [e, v] : a)
			{
				STATIC_CHECK(std::is_same_v<decltype(v), int const&>);
			}
		}
	}

	SECTION("size")
	{
		CHECK(lux::EnumArray<Enum, int>().size() == 2);
	}

	SECTION("operator[]")
	{
		// const
		{
			auto const a = lux::EnumArray<Enum, int> { 1, 2 };
			auto& v = a[Enum::A];
			STATIC_CHECK(std::is_same_v<decltype(v), int const&>);
			CHECK(v == 1);
		}

		// non const
		{
			auto a = lux::EnumArray<Enum, int> { 1, 2 };
			auto& v = a[Enum::A];
			v = 3;
			CHECK(a[Enum::A] == 3);
		}
	}

	SECTION("operator<=>")
	{
		auto const a = lux::EnumArray<Enum, int> { 1, 2 };
		auto const b = lux::EnumArray<Enum, int> { 3, 4 };
		CHECK(a <=> b == std::strong_ordering::less);
		CHECK_FALSE(a == b);
		CHECK(a != b);
		CHECK(a < b);
		CHECK_FALSE(a > b);
		CHECK(a <= b);
		CHECK_FALSE(a >= b);
	}

	SECTION("constexpr")
	{
		STATIC_CHECK(lux::EnumArray<Enum, int>().data()[0] == 0);

		[]() consteval {
			auto const a = lux::EnumArray<Enum, int>();
			for (auto [e, v] : a) {}
		}();

		STATIC_CHECK(lux::EnumArray<Enum, int>().size() == 2);
		STATIC_CHECK(lux::EnumArray<Enum, int>()[Enum::A] == 0);
		STATIC_CHECK(lux::EnumArray<Enum, int>() <=> lux::EnumArray<Enum, int>() == std::strong_ordering::equal);
	}
}

TEST_CASE("EnumArrayIterator", "[Core]")
{
	SECTION("operator==")
	{
		constexpr auto a = lux::EnumArray<Enum, int>();
		constexpr auto b = lux::EnumArray<Enum, int>();
		STATIC_CHECK(a.begin() == a.begin());
		STATIC_CHECK(a.begin() != b.begin());
	}

	auto a = lux::EnumArray<Enum, int const> { 1, 2 };
	auto it = a.begin();
	CHECK(*it++ == std::pair<Enum, int const&>(Enum::A, 1));
	CHECK(++it == a.end());
	CHECK(*--it == std::pair<Enum, int const&>(Enum::B, 2));
	CHECK(*it-- == std::pair<Enum, int const&>(Enum::B, 2));
}
