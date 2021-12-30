cmake_minimum_required (VERSION 2.8.11)
project(nirc)

add_executable(nirc
	src/main.cpp

	include/nirc/Server.hpp
	src/nirc/Server.cpp

	include/nirc/cli/Options.hpp
	include/nirc/cli/InvalidOptionsException.hpp
	src/nirc/cli/Options.cpp
)

target_include_directories(nirc PUBLIC include/)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -O2 -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -O2 -std=c++20")

