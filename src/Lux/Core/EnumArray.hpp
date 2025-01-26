#pragma once

#include "EnumUtils.hpp"
#include "Debug.hpp"

#include <utility>
#include <compare>

namespace lux
{
	template<MaxedEnum E, typename T>
	class EnumArrayIterator
	{
	public:
		constexpr EnumArrayIterator(T* data, std::size_t index) : m_data(data), m_index(index) {}

		constexpr auto operator==(EnumArrayIterator const&) const -> bool = default;

		constexpr auto operator*() { return std::pair<E, T&>(static_cast<E>(m_index), m_data[m_index]); }

		constexpr auto operator++() -> EnumArrayIterator& { ++m_index; return *this; }
		constexpr auto operator++(int) { auto tmp = *this; ++*this; return tmp; }
		constexpr auto operator--() -> EnumArrayIterator& { --m_index; return *this; }
		constexpr auto operator--(int) { auto tmp = *this; --*this; return tmp; }

	private:
		T* m_data;
		std::size_t m_index;
	};

	template<MaxedEnum E, typename T>
	class EnumArray
	{
	public:
		constexpr auto data() const -> T const* { return m_data; }
		constexpr auto data() -> T* { return m_data; }

		constexpr auto begin() const { return EnumArrayIterator<E, T const>(m_data, 0); }
		constexpr auto begin() { return EnumArrayIterator<E, T>(m_data, 0); }
		constexpr auto end() const { return EnumArrayIterator<E, T const>(m_data, size()); }
		constexpr auto end() { return EnumArrayIterator<E, T>(m_data, size()); }

		constexpr auto size() const -> std::size_t { return std::size(m_data); }

		constexpr auto operator[](E e) const -> T const& { LUX_ASSERT(std::underlying_type_t<E>(e) < size()); return m_data[std::underlying_type_t<E>(e)]; }
		constexpr auto operator[](E e) -> T& { LUX_ASSERT(std::underlying_type_t<E>(e) < size()); return m_data[std::underlying_type_t<E>(e)]; }

		constexpr auto operator<=>(EnumArray const&) const = default;

		// not private to support aggregate init
		T m_data[EnumCount<E>];
	};
}
