#include <iostream>
#include <nirc/cli/Options.hpp>
#include <nirc/cli/InvalidOptionsException.hpp>

namespace nirc::cli {
	Options::Options(int argc, char *argv[]) {
		if (argc != 3) {
			throw InvalidOptionsException(std::string("Usage: ") + argv[0] + " <hostname> <port>");
		}

		try {
			this->hostname = argv[1];
			this->portNumber = std::stoi(argv[2]);
			if (this->portNumber < 1024 || this->portNumber > 65535) {
				throw std::out_of_range("Not a valid port number");
			}
		} catch (const std::invalid_argument&) {
			throw InvalidOptionsException(std::string("Port number \"") + argv[1] + "\" is not a valid integer");
		} catch (const std::out_of_range&) {
			throw InvalidOptionsException(std::string("Port number \"") + argv[1] + "\" is out of range");
		} catch (...) {
			throw InvalidOptionsException("Unknown error has ocurred\n");
		}
	}

	int Options::getPortNumber() const {
		return this->portNumber;
	}

	int Options::getMaxClientsNumber() const {
		return this->maxClientsNumber;
	}

	const std::string& Options::getHostname() const {
		return this->hostname;
	}
}

