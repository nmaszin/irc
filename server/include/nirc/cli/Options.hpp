#pragma once

#include <string>

namespace nirc::cli {
	class Options {
	public:
		Options(int argc, char *argv[]);
		int getPortNumber() const;
		int getMaxClientsNumber() const;
		const std::string& getHostname() const;

	private:
		std::string hostname;
		int portNumber;
		int maxClientsNumber = 10;
	};
}

