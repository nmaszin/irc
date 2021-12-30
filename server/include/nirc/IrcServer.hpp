#pragma once

#include <string>
#include <memory>
#include <nirc/cli/Options.hpp>
#include <nirc/network/TcpSocket.hpp>

namespace nirc {
	class IrcServer {
	public:
		IrcServer(const nirc::cli::Options& options);
		void run();

	protected:
		static void handleClient(std::unique_ptr<nirc::network::TcpSocket>&& client);

		const cli::Options& options;
	};
}
