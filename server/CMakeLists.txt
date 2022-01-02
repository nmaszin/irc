cmake_minimum_required (VERSION 2.8.11)
project(nirc)

add_executable(nirc
	src/main.cpp

	include/nirc/IrcServer.hpp
	src/nirc/IrcServer.cpp

	include/nirc/cli/Options.hpp
	include/nirc/cli/InvalidOptionsException.hpp
	src/nirc/cli/Options.cpp
	src/nirc/cli/InvalidOptionsException.cpp

	include/nirc/network/AddressInfo.hpp
	include/nirc/network/TcpServer.hpp
	include/nirc/network/TcpSocket.hpp
	include/nirc/network/TcpServerConfig.hpp
	include/nirc/network/TcpException.hpp
	src/nirc/network/TcpServerConfig.cpp

	include/nirc/network/bsd/BsdAddressInfo.hpp
	include/nirc/network/bsd/BsdTcpServer.hpp
	include/nirc/network/bsd/BsdTcpSocket.hpp
	src/nirc/network/bsd/BsdAddressInfo.cpp
	src/nirc/network/bsd/BsdTcpServer.cpp
	src/nirc/network/bsd/BsdTcpSocket.cpp

	include/nirc/irc/InputIrcMessage.hpp
	include/nirc/irc/OutputIrcMessage.hpp
	include/nirc/irc/IrcMessagePrefix.hpp
	include/nirc/irc/IrcMessageException.hpp
	include/nirc/irc/IrcMessageSender.hpp
	src/nirc/irc/InputIrcMessage.cpp
	src/nirc/irc/OutputIrcMessage.cpp
	src/nirc/irc/IrcMessagePrefix.cpp
	src/nirc/irc/IrcMessageSender.cpp

	include/nirc/irc/commands/all.hpp
	include/nirc/irc/commands/Command.hpp
	include/nirc/irc/commands/CommandException.hpp
	include/nirc/irc/commands/Join.hpp
	include/nirc/irc/commands/Nick.hpp
	include/nirc/irc/commands/PrivMsg.hpp
	include/nirc/irc/commands/User.hpp
	src/nirc/irc/commands/Command.cpp
	src/nirc/irc/commands/Join.cpp
	src/nirc/irc/commands/Nick.cpp
	src/nirc/irc/commands/PrivMsg.cpp
	src/nirc/irc/commands/User.cpp

	include/nirc/irc/handler/InputMessageHandler.hpp
	include/nirc/irc/handler/MessageHandlerException.hpp
	src/nirc/irc/handler/InputMessageHandler.cpp

	include/nirc/utils.hpp
)

set_property(TARGET nirc PROPERTY CXX_STANDARD 17)
target_include_directories(nirc PUBLIC include/)
target_link_libraries(nirc PUBLIC pthread)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -O2 -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -O2 -std=c++20")
