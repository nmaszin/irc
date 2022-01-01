#pragma once

#include <string>

namespace nirc::cli {
	class Options {
	public:
		Options(int argc, char *argv[]);
		int getPortNumber() const;
		const std::string& getHostname() const;

	private:
		std::string hostname;
		int portNumber;
	};
}

