#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <nirc/cli/Options.hpp>
#include <nirc/cli/InvalidOptionsException.hpp>
#include <nirc/network/TcpException.hpp>
#include <nirc/irc/IrcServer.hpp>

int main(int argc, char *argv[]) {
	try {
		nirc::cli::Options options(argc, argv);
		nirc::irc::IrcServer server(options);
		server.run();
	} catch (const nirc::cli::InvalidOptionsException& e) {
		std::cerr << "Incorrect usage!\n";
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	} catch (const std::exception& e) {
		std::cerr << "Unknown error ocurred\n";
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}

