#include <iostream>
#include <memory>
#include <thread>
#include <list>
#include <nirc/Server.hpp>
#include <nirc/network/TcpServer.hpp>
#include <nirc/network/TcpSocket.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/network/bsd/BsdTcpServer.hpp>

namespace nirc {
	Server::Server(const nirc::cli::Options& options) :
		options(options)
	{
	}

	void Server::run() {
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

	void Server::handleClient(std::unique_ptr<nirc::network::TcpSocket>&& client) {
		try {
			while (true) {
				client->send("Hello ;)\n");
			}
		} catch (const nirc::network::TcpException& e) {
			// Do nothing, only close the connection
		}
	}
}
