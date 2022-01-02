#pragma once

#include <string>
#include <memory>
#include <nirc/cli/Options.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/irc/IrcMessagePrefix.hpp>
#include <nirc/irc/handler/InputMessageHandler.hpp>

namespace nirc {
	class IrcServer {
	public:
		IrcServer(const nirc::cli::Options& options);
		void run();

	protected:
		void handleClient(std::unique_ptr<nirc::network::TcpSocket>&& client);

		const cli::Options& options;
		irc::IrcMessageServerPrefix serverPrefix;
		irc::handler::InputMessageHandler messageHandler;
	};
}
