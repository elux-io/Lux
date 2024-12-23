#pragma once

#include <type_traits>
#include <concepts>

#define LUX_ENABLE_FLAGS(Enum, flagsConfig) \
	consteval auto LuxFlagsConfig(Enum) -> ::lux::FlagsConfig \
	{ \
		return flagsConfig; \
	}

namespace lux
{
	struct FlagsConfig
	{
		bool isPow2 = false;
	};

	template<typename T>
	concept FlagEnum = std::is_enum_v<T> and requires(T t)
	{
		{ LuxFlagsConfig(t) } -> std::same_as<FlagsConfig>;
	};

	template<FlagEnum E>
	class Flags
	{
	public:
		using Bits = std::underlying_type_t<E>;

		constexpr Flags() = default;
		constexpr Flags(E flag) : m_bits(GetBitValue(flag)) {}
		constexpr explicit Flags(Bits bits) : m_bits(bits) {}

		constexpr auto Has(Flags flags) const { return (*this & flags) == flags; }
		constexpr auto Set(Flags flags) { *this |= flags; }
		constexpr auto Unset(Flags flags) { m_bits &= ~flags.m_bits; }
		constexpr auto IsEmpty() const { return m_bits == Bits(); }
		constexpr auto GetBits() const { return m_bits; }

		constexpr auto& operator&=(Flags b) { m_bits &= b.m_bits; return *this; }
		constexpr auto& operator|=(Flags b) { m_bits |= b.m_bits; return *this; }
		constexpr auto& operator^=(Flags b) { m_bits ^= b.m_bits; return *this; }

		friend constexpr auto operator==(Flags, Flags) -> bool = default;
		friend constexpr auto operator&(Flags a, Flags b) { return a &= b; }
		friend constexpr auto operator|(Flags a, Flags b) { return a |= b; }
		friend constexpr auto operator^(Flags a, Flags b) { return a ^= b; }

	private:
		static constexpr auto s_config = LuxFlagsConfig(E());

		static constexpr auto GetBitValue(E flag) -> Bits
		{
			if constexpr (s_config.isPow2)
				return static_cast<Bits>(flag);
			else
				return Bits(1) << static_cast<Bits>(flag);
		}

		Bits m_bits {};
	};
}

template<lux::FlagEnum E> constexpr auto operator&(E a, E b) { return lux::Flags(a) & b; }
template<lux::FlagEnum E> constexpr auto operator|(E a, E b) { return lux::Flags(a) | b; }
template<lux::FlagEnum E> constexpr auto operator^(E a, E b) { return lux::Flags(a) ^ b; }
