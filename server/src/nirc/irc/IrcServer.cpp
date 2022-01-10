#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>
#include <nirc/irc/IrcServer.hpp>
#include <nirc/irc/ServerMonitor.hpp>
#include <nirc/irc/message/InputIrcMessage.hpp>
#include <nirc/irc/message/OutputIrcMessage.hpp>
#include <nirc/irc/message/MessageParsingException.hpp>
#include <nirc/irc/state/StateException.hpp>
#include <nirc/irc/commands/all.hpp>

namespace nirc::irc {
	IrcServer::IrcServer(const nirc::cli::Options& options) :
		serverState(options),
		messageHandler(irc::commands::all())
	{
	}

	void IrcServer::run() {
		const auto& options = serverState.getOptions();;
		nirc::network::TcpServerConfig config(options.getPortNumber(), options.getMaxClientsNumber());
		std::unique_ptr<nirc::network::TcpServer> server(new nirc::network::bsd::BsdTcpServer());
	
		server->run(config);
		std::cout << "Running server at port " << options.getPortNumber() << '\n';

		ServerMonitor monitor(this->serverState);
		std::thread serverMonitor(monitor);

		std::list<std::thread> clientThreads;
		while (true) {
			auto socket = server->accept();
			clientThreads.push_back(std::thread(
				[this](auto&& socket) {
					this->handleClient(std::move(socket));
				},
				std::move(socket)
			));
		}
	}

	void IrcServer::handleClient(std::unique_ptr<network::TcpSocket>&& socket) {
		state::UserState& user = this->serverState.addUser(std::move(socket));
		try {
			auto& privateRespondent = user.getPrivateRespondent();
			privateRespondent.send<responses::Response::ERR_NOMOTD>();
			while (true) {
				this->handleMessage(user);
			}
		} catch (const network::TcpException&) {
			// Handle client disconnection
			this->serverState.deleteUser(user);
		}
	}

	void IrcServer::handleMessage(state::UserState& userState) {
		try {
			auto& socket = userState.getSocket();
			message::InputIrcMessage message(socket.receiveUntil("\n"));
			messageHandler.handle(userState, message);
		} catch (const message::MessageParsingException& e) {
			// Do nothing
		}catch (const responses::ResponseException&) {
			// Do nothing
			// Error message has been already sent to client
			// Exception only breaks processing the error commend
		}
	}
}
