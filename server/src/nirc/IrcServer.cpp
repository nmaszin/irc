#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/IrcServer.hpp>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>
#include <nirc/irc/RawIrcMessage.hpp>

namespace nirc {
	IrcServer::IrcServer(const nirc::cli::Options& options) :
		options(options)
	{
	}

	void IrcServer::run() {
		nirc::network::TcpServerConfig config(options.getPortNumber());
		std::unique_ptr<nirc::network::TcpServer> s(new nirc::network::bsd::BsdTcpServer());
	
		s->run(config);
		std::cout << "Running server at port " << options.getPortNumber() << '\n';

		std::list<std::thread> threads;
		while (true) {
			auto client = s->accept();
			threads.push_back(std::thread(handleClient, std::move(client), options));
		}
	}

	void IrcServer::handleClient(
		std::unique_ptr<nirc::network::TcpSocket>&& client,
		const cli::Options& options) 
	{
		try {
			std::cout << "Mamy nowego klienta ;) " << client->getInfo().getHostname() << "\n";

			irc::RawIrcMessage msg(
				options.getHostname(),
				"001",
				{"*", "Welcome to nirc server ;)"}
			);

			client->send(msg.toString());
			while (true) {
				irc::RawIrcMessage msg(client->receiveUntil("\n"));
			}
		} catch (const nirc::network::TcpException& e) {
			// Do nothing, only close the connection
		}
	}
}
