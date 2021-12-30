#include <iostream>
#include <nirc/Server.hpp>

namespace nirc {
	Server::Server(unsigned port) {
		std::cout << "Initializing server on port " << port << '\n';
	}
}

