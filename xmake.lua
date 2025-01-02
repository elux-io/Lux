set_languages("c++20")
set_warnings("allextra")

option("enable-demos", { default = false })
option("enable-tests", { default = false })

add_rules("mode.debug", "mode.release", "mode.coverage")
add_cxxflags("/Zc:preprocessor", "/Zc:templateScope", "/Zc:throwingNew", "/Zc:enumTypes", "/Zc:__cplusplus", { tools = "cl" })

target("lux")
	set_kind("static")
	add_includedirs("src", { public = true })
	add_files("src/Lux/**.cpp")

if has_config("enable-demos") then
	includes("demos/*")
end

if has_config("enable-tests") then
	includes("tests")
end
