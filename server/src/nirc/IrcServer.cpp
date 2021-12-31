#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/IrcServer.hpp>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>

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
			threads.push_back(std::thread(handleClient, std::move(client)));
		}
	}

	void IrcServer::handleClient(std::unique_ptr<nirc::network::TcpSocket>&& client) {
		try {
			client->send("Hello ;)\n");
			while (true) {
				client->send("What's your name?\n");
				auto response = client->receiveUntil("\n");
				client->send("Hello, " + response + "!\n");
			}
		} catch (const nirc::network::TcpException& e) {
			// Do nothing, only close the connection
		}
	}
}
