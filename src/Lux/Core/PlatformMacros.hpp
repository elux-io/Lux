#pragma once

/**
 * Compiler detection
 */
#define LUX_COMPILER(name) LUX_PRIVATE_COMPILER_##name()

#if defined(__clang__)
	#define LUX_PRIVATE_COMPILER_CLANG() 1
#else
	#define LUX_PRIVATE_COMPILER_CLANG() 0
#endif

#if defined(__GNUC__) and not defined(__clang__)
	#define LUX_PRIVATE_COMPILER_GCC() 1
#else
	#define LUX_PRIVATE_COMPILER_GCC() 0
#endif

#if defined(_MSC_VER)
	#define LUX_PRIVATE_COMPILER_MSVC() 1
#else
	#define LUX_PRIVATE_COMPILER_MSVC() 0
#endif

#if not LUX_COMPILER(CLANG) and \
    not LUX_COMPILER(GCC) and \
    not LUX_COMPILER(MSVC)
#pragma message("unknown compiler")
#endif

/**
 * OS detection
 */
#define LUX_OS(name) LUX_PRIVATE_OS_##name()

#if defined(_WIN32)
	#define LUX_PRIVATE_OS_WINDOWS() 1
#else
	#define LUX_PRIVATE_OS_WINDOWS() 0
#endif

#if defined(__linux__)
	#define LUX_PRIVATE_OS_LINUX() 1
#else
	#define LUX_PRIVATE_OS_LINUX() 0
#endif

#if defined(__APPLE__) and defined(__MACH__)
	#define LUX_PRIVATE_OS_MAC() 1
#else
	#define LUX_PRIVATE_OS_MAC() 0
#endif

#if not LUX_OS(WINDOWS) and \
    not LUX_OS(LINUX) and \
    not LUX_OS(MAC)
#pragma message("unknown OS")
#endif

/**
 * C++ version detection
 */
#define LUX_CPP(version) LUX_PRIVATE_CPP_##version()

#if __cplusplus >= 202002L
	#define LUX_PRIVATE_CPP_20() 1
#else
	#define LUX_PRIVATE_CPP_20() 0
#endif

#if __cplusplus >= 202302L
	#define LUX_PRIVATE_CPP_23() 1
#else
	#define LUX_PRIVATE_CPP_23() 0
#endif
