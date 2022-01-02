#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>
#include <nirc/irc/IrcServer.hpp>
#include <nirc/irc/ClientContext.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/handler/MessageHandlerException.hpp>
#include <nirc/irc/commands/all.hpp>

namespace nirc::irc {
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
			ClientContext context(s->accept());
			threads.push_back(std::thread(
				[this](auto&& context) {
					this->handleClient(std::move(context));
				},
				std::move(context)
			));
		}
	}

	void IrcServer::handleClient(ClientContext&& context) {
		try {
			while (true) {
				this->handleMessage(context);
			}
		} catch (const nirc::network::TcpException& e) {
			// Do nothing, only close the connection
		}
	}

	void IrcServer::handleMessage(ClientContext& context) {
		try {
			irc::message::InputIrcMessage msg(context.getSocket().receiveUntil("\n"));
			messageHandler.handle(context, msg);
		} catch (const handler::MessageHandlerException&) {
			std::cerr << "Invalid command\n";
		}
	}
}
