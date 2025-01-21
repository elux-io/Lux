#pragma once

#include <type_traits>
#include <concepts>

#define LUX_SET_ENUM_MAX(Enum, Max) \
	consteval auto LuxEnumMax(Enum) -> Enum \
	{ \
		return Enum::Max; \
	}

namespace lux
{
	template<typename T>
	concept MaxedEnum = std::is_enum_v<T> and (
		requires { T::Max_; } or
		requires (T t)
		{
			{ LuxEnumMax(t) } -> std::same_as<T>;
		});

	namespace detail
	{
		template<MaxedEnum E>
		consteval auto GetEnumMax() -> E
		{
			if constexpr (requires { E::Max_; })
				return E::Max_;
			else
				return LuxEnumMax(E());
		}
	}

	template<MaxedEnum E>
	inline constexpr auto EnumMax = detail::GetEnumMax<E>();

	template<MaxedEnum E>
	inline constexpr auto EnumCount = std::size_t(static_cast<std::size_t>(EnumMax<E>) + std::size_t(1));
}
