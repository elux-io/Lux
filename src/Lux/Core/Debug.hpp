#pragma once

#include "PlatformMacros.hpp"

#include <Lux/Log/Log.hpp>

#if LUX_COMPILER(MSVC)
	#define LUX_DEBUG_BREAK() __debugbreak()
#elif LUX_COMPILER(CLANG) or LUX_COMPILER(GCC)
	#define LUX_DEBUG_BREAK() __builtin_trap()
#else
	#define LUX_DEBUG_BREAK() ((void)0)
#endif

#if LUX_CPP(23)
	#define LUX_ASSUME(expr) [[assume(expr)]]
#else
	#if LUX_COMPILER(MSVC)
		#define LUX_ASSUME(expr) __assume(expr)
	#elif LUX_COMPILER(CLANG)
		#define LUX_ASSUME(expr) __builtin_assume(expr)
	#elif LUX_COMPILER(GCC)
		#define LUX_ASSUME(expr) __attribute__((assume(expr)))
	#else
		#define LUX_ASSUME(...) ((void)0)
	#endif
#endif

#if not defined(NDEBUG)
	#define LUX_ASSERT(expr, ...) ((expr) or (::lux::detail::DO_NOT_PAY_ATTENTION_TO_WHATEVER_THE_HELL_THE_COMPILER_IS_SAYING__THIS_ERROR_IS_CAUSED_BY_A_FAILED_ASSERT(), LUX_LOG_ERROR("assertion `" #expr "` failed" __VA_OPT__(": ") __VA_ARGS__), LUX_DEBUG_BREAK(), false))
#else
	#define LUX_ASSERT(expr, ...) LUX_ASSUME(expr)
#endif

namespace lux
{
	namespace detail
	{
		inline auto DO_NOT_PAY_ATTENTION_TO_WHATEVER_THE_HELL_THE_COMPILER_IS_SAYING__THIS_ERROR_IS_CAUSED_BY_A_FAILED_ASSERT() {}
	}

	[[noreturn]] inline auto Unreachable() -> void
	{
		LUX_LOG_ERROR("unreachable code reached");
		LUX_DEBUG_BREAK();

		#if LUX_COMPILER(MSVC)
			__assume(false);
		#elif LUX_COMPILER(CLANG) or LUX_COMPILER(GCC)
			__builtin_unreachable();
		#endif
	}
}
