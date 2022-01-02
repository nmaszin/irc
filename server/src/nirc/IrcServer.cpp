#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/IrcServer.hpp>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>
#include <nirc/irc/IrcMessageSender.hpp>
#include <nirc/irc/InputIrcMessage.hpp>
#include <nirc/irc/OutputIrcMessage.hpp>
#include <nirc/irc/commands/all.hpp>
#include <nirc/irc/handler/MessageHandlerException.hpp>

namespace nirc {
	IrcServer::IrcServer(const nirc::cli::Options& options) :
		options(options),
		serverPrefix(options.getHostname()),
		messageHandler(irc::commands::all())
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
			threads.push_back(std::thread(
				[this](std::unique_ptr<nirc::network::TcpSocket>&& client) {
					this->handleClient(std::move(client));
				},
				std::move(client)
			));
		}
	}

	void IrcServer::handleClient(std::unique_ptr<nirc::network::TcpSocket>&& client) {
		try {
			irc::IrcMessageUserPrefix clientPrefix("nick", "username", "hostname");
			irc::IrcMessageSender s(*client, serverPrefix);
			irc::IrcMessageSender c(*client, clientPrefix);

			s.send("001", {"*", "Welcome to nirc server ;)"});
			c.send("PRIVMSG", {"#kanal", "Siema :D"});

			while (true) {
				irc::InputIrcMessage msg(client->receiveUntil("\n"));
				messageHandler.handle(s, msg);
			}
		} catch (const nirc::network::TcpException& e) {
			// Do nothing, only close the connection
		}
	}
}
