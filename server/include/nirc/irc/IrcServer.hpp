#pragma once

#include <string>
#include <memory>

#include <nirc/cli/Options.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/Prefix.hpp>
#include <nirc/irc/handler/InputMessageHandler.hpp>

namespace nirc::irc {
	class IrcServer {
	public:
		IrcServer(const nirc::cli::Options& options);
		void run();

	protected:
		void handleClient(ClientContext&& context);
		void handleMessage(ClientContext& context);

		cli::Options options;
		state::ServerState serverState;
		handler::InputMessageHandler messageHandler;
	};
}
