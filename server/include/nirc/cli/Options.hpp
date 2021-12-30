#pragma once

#include <string>

namespace nirc::cli {
	class Options {
	public:
		Options(int argc, char *argv[]);
		int getPortNumber() const;

	private:
		int portNumber;
	};
}

