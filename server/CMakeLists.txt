cmake_minimum_required (VERSION 2.8.11)
project(nirc)

add_executable(nirc
	src/main.cpp

	include/nirc/Server.hpp
	src/nirc/Server.cpp

	include/nirc/cli/Options.hpp
	include/nirc/cli/InvalidOptionsException.hpp
	src/nirc/cli/Options.cpp

	include/nirc/network/TcpServer.hpp
	include/nirc/network/TcpSocket.hpp
	include/nirc/network/TcpServerConfig.hpp
	include/nirc/network/TcpException.hpp
	src/nirc/network/TcpServerConfig.cpp

	include/nirc/network/bsd/BsdTcpServer.hpp
	include/nirc/network/bsd/BsdTcpSocket.hpp
	src/nirc/network/bsd/BsdTcpServer.cpp
	src/nirc/network/bsd/BsdTcpSocket.cpp
)

set_property(TARGET nirc PROPERTY CXX_STANDARD 17)
target_include_directories(nirc PUBLIC include/)
target_link_libraries(nirc PUBLIC pthread)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -O2 -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -O2 -std=c++20")

