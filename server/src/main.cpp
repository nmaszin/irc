#include <iostream>
#include <cstdlib>
#include <nirc/cli/Options.hpp>
#include <nirc/cli/InvalidOptionsException.hpp>
#include <nirc/Server.hpp>

int main(int argc, char *argv[]) {
	try {
		nirc::cli::Options options(argc, argv);
		int port = options.getPortNumber();

		nirc::Server server(port);
	} catch (const InvalidOptionsException& e) {
		std::cerr << "Incorrect usage!\n";
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	} catch (...) {
		std::cerr << "Unknown error ocurred\n";
		return EXIT_FAILURE;
	}
}

