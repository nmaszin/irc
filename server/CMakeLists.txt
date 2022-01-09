cmake_minimum_required (VERSION 2.8.11)
project(nirc)

add_executable(nirc
	src/main.cpp

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
	src/nirc/network/TcpException.cpp

	include/nirc/network/bsd/BsdAddressInfo.hpp
	include/nirc/network/bsd/BsdTcpServer.hpp
	include/nirc/network/bsd/BsdTcpSocket.hpp
	src/nirc/network/bsd/BsdAddressInfo.cpp
	src/nirc/network/bsd/BsdTcpServer.cpp
	src/nirc/network/bsd/BsdTcpSocket.cpp

	include/nirc/irc/IrcServer.hpp
	include/nirc/irc/ServerMonitor.hpp
	src/nirc/irc/IrcServer.cpp
	src/nirc/irc/ServerMonitor.cpp

	include/nirc/irc/message/InputIrcMessage.hpp
	include/nirc/irc/message/OutputIrcMessage.hpp
	include/nirc/irc/message/Prefix.hpp
	src/nirc/irc/message/InputIrcMessage.cpp
	src/nirc/irc/message/OutputIrcMessage.cpp
	src/nirc/irc/message/Prefix.cpp

	include/nirc/irc/state/ServerState.hpp
	include/nirc/irc/state/UserState.hpp
	include/nirc/irc/state/ChannelState.hpp
	include/nirc/irc/state/StateException.hpp
	src/nirc/irc/state/ServerState.cpp
	src/nirc/irc/state/UserState.cpp
	src/nirc/irc/state/StateException.cpp
	src/nirc/irc/state/ChannelState.cpp

	include/nirc/irc/commands/all.hpp
	include/nirc/irc/commands/Command.hpp
	include/nirc/irc/commands/Join.hpp
	include/nirc/irc/commands/Nick.hpp
	include/nirc/irc/commands/PrivMsg.hpp
	include/nirc/irc/commands/User.hpp
	include/nirc/irc/commands/IsOn.hpp
	include/nirc/irc/commands/Quit.hpp
	include/nirc/irc/commands/Part.hpp
	include/nirc/irc/commands/Topic.hpp
	include/nirc/irc/commands/List.hpp
	include/nirc/irc/commands/Names.hpp
	include/nirc/irc/commands/Mode.hpp
	include/nirc/irc/commands/Kick.hpp
	include/nirc/irc/commands/Cap.hpp
	include/nirc/irc/commands/Ping.hpp
	include/nirc/irc/commands/Pong.hpp
	include/nirc/irc/commands/Who.hpp
	src/nirc/irc/commands/Command.cpp
	src/nirc/irc/commands/Join.cpp
	src/nirc/irc/commands/Nick.cpp
	src/nirc/irc/commands/PrivMsg.cpp
	src/nirc/irc/commands/User.cpp
	src/nirc/irc/commands/IsOn.cpp
	src/nirc/irc/commands/Quit.cpp
	src/nirc/irc/commands/Part.cpp
	src/nirc/irc/commands/Topic.cpp
	src/nirc/irc/commands/List.cpp
	src/nirc/irc/commands/Names.cpp
	src/nirc/irc/commands/Mode.cpp
	src/nirc/irc/commands/Kick.cpp
	src/nirc/irc/commands/Cap.cpp
	src/nirc/irc/commands/Ping.cpp
	src/nirc/irc/commands/Pong.cpp
	src/nirc/irc/commands/Who.cpp

	include/nirc/irc/handler/InputMessageHandler.hpp
	include/nirc/irc/handler/MessageHandlerException.hpp
	src/nirc/irc/handler/InputMessageHandler.cpp

	include/nirc/irc/responses/Response.hpp
	include/nirc/irc/responses/ResponseException.hpp
	include/nirc/irc/responses/PrivateRespondent.hpp
	include/nirc/irc/responses/PrivateResponseGenerator.hpp
	include/nirc/irc/responses/BroadcastRespondent.hpp
	include/nirc/irc/responses/BroadcastResponseGenerator.hpp
	src/nirc/irc/responses/PrivateResponseGenerator.cpp
	src/nirc/irc/responses/BroadcastResponseGenerator.cpp

	include/nirc/utils/string.hpp
	include/nirc/utils/Serializable.hpp
	src/nirc/utils/string.cpp
)

set_property(TARGET nirc PROPERTY CXX_STANDARD 17)
target_include_directories(nirc PUBLIC include/)
target_link_libraries(nirc PUBLIC pthread)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -O2 -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Wpedantic -O2 -std=c++20")
