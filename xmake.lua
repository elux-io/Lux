includes("**/xmake.lua")
add_rules("mode.debug", "mode.release")

set_languages("c++20")
set_warnings("allextra")

add_cxxflags("/Zc:preprocessor", "/Zc:templateScope", "/Zc:throwingNew", "/Zc:enumTypes", { tools = "cl" })

target("lux")
	set_kind("static")
	add_includedirs("src", { public = true })
	add_files("src/Lux/**.cpp")

target("main")
	set_kind("binary")
	add_deps("lux")
	add_files("src/main.cpp")
--
