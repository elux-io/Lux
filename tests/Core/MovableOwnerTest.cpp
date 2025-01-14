#include "../Utils.hpp"

#include <Lux/Core/MovableOwner.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("MovableOwner", "[Core]")
{
	SECTION("only works on integrals/pointers")
	{
		struct S {};

		[]<typename T> {
			CHECK_DOES_NOT_COMPILE(lux::MovableOwner<T>());
		}.operator()<S>();
	}

	SECTION("ctor")
	{
		// integral
		{
			CHECK(lux::MovableOwner<int>().Get() == 0);
			CHECK(lux::MovableOwner(5).Get() == 5);
			CHECK(lux::MovableOwner<int, -1>().Get() == -1);
			CHECK(lux::MovableOwner<int, -1>(5).Get() == 5);

			// implicit conversion
			{
				auto const a = 'a';
				lux::MovableOwner<int> const b = a;
				CHECK(b.Get() == 'a');
			}
		}

		// pointer
		{
			{
				auto const obj = 2;
				auto const a = lux::MovableOwner(&obj);
				CHECK(a.Get() == &obj);
			}

			// implicit conversion
			{
				struct Base {};
				struct Derived : Base {};

				auto const obj = Derived();
				auto const ptr = &obj;
				lux::MovableOwner<Base const*> const a = ptr;
				CHECK(a.Get() == ptr);
			}
		}
	}

	SECTION("copy ctor")
	{
		auto const a = lux::MovableOwner(5);
		auto const b = a;

		CHECK(a.Get() == 5);
		CHECK(b.Get() == 5);
	}

	SECTION("copy assign")
	{
		auto const a = lux::MovableOwner(5);
		auto b = lux::MovableOwner(0);
		b = a;

		CHECK(a.Get() == 5);
		CHECK(b.Get() == 5);
	}

	SECTION("move ctor")
	{
		// integral
		{
			{
				auto a = lux::MovableOwner(5);
				auto const b = std::move(a);

				CHECK(a.Get() == 0);
				CHECK(b.Get() == 5);
			}

			// with custom empty value
			{
				auto a = lux::MovableOwner<int, -1>(5);
				auto const b = std::move(a);

				CHECK(a.Get() == -1);
				CHECK(b.Get() == 5);
			}
		}

		// pointer
		{
			auto obj = 2;
			auto a = lux::MovableOwner(&obj);
			auto b = std::move(a);

			CHECK(a.Get() == nullptr);
			CHECK(b.Get() == &obj);
		}
	}

	SECTION("move assign")
	{
		// integral
		{
			{
				auto a = lux::MovableOwner(5);
				auto b = lux::MovableOwner(0);
				b = std::move(a);

				CHECK(a.Get() == 0);
				CHECK(b.Get() == 5);
			}

			// with custom empty value
			{
				auto a = lux::MovableOwner<int, -1>(5);
				auto b = lux::MovableOwner<int, -1>();
				b = std::move(a);

				CHECK(a.Get() == -1);
				CHECK(b.Get() == 5);
			}
		}

		// pointer
		{
			auto obj = 2;
			auto a = lux::MovableOwner(&obj);
			auto b = lux::MovableOwner<int*>();
			b = std::move(a);

			CHECK(a.Get() == nullptr);
			CHECK(b.Get() == &obj);
		}
	}

	SECTION("member access operators")
	{
		// only work on pointers
		[]<typename T = int> {
			auto const a = lux::MovableOwner<T>();
			auto b = lux::MovableOwner<T>();

			CHECK_DOES_NOT_COMPILE(*a);
			CHECK_DOES_NOT_COMPILE(*b);
			CHECK_DOES_NOT_COMPILE(a.operator->());
			CHECK_DOES_NOT_COMPILE(b.operator->());
		}();

		// basic usage
		{
			auto const i = 5;
			auto const a = lux::MovableOwner(&i);
			auto b = lux::MovableOwner(&i);

			CHECK(*a == 5);
			CHECK(*b == 5);
			CHECK(*a.operator->() == 5);
			CHECK(*b.operator->() == 5);
		}

		// return type is correct
		{
			// const
			{
				auto const a = lux::MovableOwner<int*>();
				STATIC_CHECK(std::is_same_v<decltype(*a), int const&>);
				STATIC_CHECK(std::is_same_v<decltype(a.operator->()), int const*>);
			}

			// non const
			{
				auto a = lux::MovableOwner<int*>();
				STATIC_CHECK(std::is_same_v<decltype(*a), int&>);
				STATIC_CHECK(std::is_same_v<decltype(a.operator->()), int*>);
			}

			// with pointer to const
			{
				auto const a = lux::MovableOwner<int const*>();
				auto b = lux::MovableOwner<int const*>();

				STATIC_CHECK(std::is_same_v<decltype(*a), int const&>);
				STATIC_CHECK(std::is_same_v<decltype(*b), int const&>);
				STATIC_CHECK(std::is_same_v<decltype(a.operator->()), int const*>);
				STATIC_CHECK(std::is_same_v<decltype(b.operator->()), int const*>);
			}
		}
	}

	SECTION("conversion operator")
	{
		auto const a = lux::MovableOwner(5);
		CHECK(a == 5);

		auto const obj = 2;
		auto const b = lux::MovableOwner(&obj);
		CHECK(b == &obj);
	}

	SECTION("constexpr")
	{
		// ctors
		{
			[[maybe_unused]] constexpr auto a = lux::MovableOwner<int>();
			[[maybe_unused]] constexpr auto b = lux::MovableOwner(5);
			[[maybe_unused]] constexpr auto c = lux::MovableOwner<int>('a');
		}

		// copy ctor
		{
			constexpr auto a = lux::MovableOwner(5);
			[[maybe_unused]] constexpr auto b = a;
		}

		// move ctor
		STATIC_CHECK([] {
			auto a = lux::MovableOwner(5);
			auto const b = std::move(a);
			return b;
		}().Get() == 5);

		// copy assign
		STATIC_CHECK([] {
			auto const a = lux::MovableOwner(5);
			auto b = lux::MovableOwner(0);
			b = a;
			return b;
		}().Get() == 5);

		// move assign
		STATIC_CHECK([] {
			auto a = lux::MovableOwner(5);
			auto b = lux::MovableOwner(0);
			b = std::move(a);
			return b;
		}().Get() == 5);

		// conversion operator
		STATIC_CHECK(lux::MovableOwner(5) == 5);

		// member access operators
		[]() consteval {
			auto const i = 5;
			auto const a = lux::MovableOwner(&i);
			auto b = lux::MovableOwner(&i);

			[[maybe_unused]] auto _1 = *a;
			[[maybe_unused]] auto _2 = *b;
			[[maybe_unused]] auto _3 = a.operator->();
			[[maybe_unused]] auto _4 = b.operator->();
		}();
	}
}
