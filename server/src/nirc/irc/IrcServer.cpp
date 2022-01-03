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
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/commands/all.hpp>

namespace nirc::irc {
	IrcServer::IrcServer(const nirc::cli::Options& options) :
		serverState(options),
		messageHandler(irc::commands::all())
	{
	}

	void IrcServer::run() {
		const auto& options = serverState.options;
		nirc::network::TcpServerConfig config(options.getPortNumber(), options.getMaxClientsNumber());
		std::unique_ptr<nirc::network::TcpServer> server(new nirc::network::bsd::BsdTcpServer());
	
		server->run(config);
		std::cout << "Running server at port " << options.getPortNumber() << '\n';

		std::thread serverMonitor([this] () {
			while (true) {
				std::cout << "Available users:\n";
				int i = 1;
				for (const auto& e : this->serverState.users) {
					if (e) {
						const auto& user = *e;
						if (user.nick) {
							std::cout << i << ". " << *user.nick << "\n";
						} else {
							std::cout << i << ". " << "user uninitialized" << "\n";
						}
						i++;
					}
				}

				using namespace std::chrono;
				std::this_thread::sleep_for(5s);
			}
		});


		std::list<std::thread> threads;
		while (true) {
			auto socket = server->accept();
			threads.push_back(std::thread(
				[this](auto&& socket) {
					this->handleClient(std::move(socket));
				},
				std::move(socket)
			));
		}
	}

	void IrcServer::handleClient(std::unique_ptr<network::TcpSocket>&& socket) {
		try {
			ClientContext context(this->serverState, std::move(socket));
			while (true) {
				this->handleMessage(context);
			}
		} catch (const state::StateException& e) {
			// Do nothing, only close the connection
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
