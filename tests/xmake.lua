add_requires("catch2")

for _, dir in ipairs(os.dirs("./**")) do
	local test_files = dir .. "/**Test.cpp"

	target(dir .. "Tests")
		set_kind("binary")
		set_default(false)
		add_packages("catch2")
		add_deps("lux")
		add_files(test_files)
		add_tests("tests")
end
