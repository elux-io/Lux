#pragma once

#include <type_traits>
#include <utility>

namespace lux
{
	template<typename T, T EmptyValue = T()>
		requires std::is_integral_v<T> or std::is_pointer_v<T>
	class MovableOwner
	{
	public:
		constexpr MovableOwner() = default;
		constexpr MovableOwner(T value) : m_value(value) {}
		constexpr MovableOwner(MovableOwner const&) = default;
		constexpr MovableOwner(MovableOwner&& other) noexcept : m_value(std::exchange(other.m_value, EmptyValue)) {}
		constexpr ~MovableOwner() = default;

		constexpr auto Get() const { return m_value; }
		constexpr operator auto() const { return m_value; }

		constexpr auto operator*() const -> std::remove_pointer_t<T> const& requires std::is_pointer_v<T> { return *m_value; }
		constexpr auto operator*() -> std::remove_pointer_t<T>& requires std::is_pointer_v<T> { return *m_value; }
		constexpr auto operator->() const -> std::remove_pointer_t<T> const* requires std::is_pointer_v<T> { return m_value; }
		constexpr auto operator->() -> T requires std::is_pointer_v<T> { return m_value; }

		constexpr auto operator=(MovableOwner const&) -> MovableOwner& = default;
		constexpr auto operator=(MovableOwner&& other) noexcept -> MovableOwner& { std::swap(m_value, other.m_value); return *this; }

	private:
		T m_value = EmptyValue;
	};
}
