#pragma once

#include <stdexcept>
#include <string>

namespace nirc::cli {
	class InvalidOptionsException : public std::runtime_error {
	public:
		InvalidOptionsException(const std::string& what) :
			std::runtime_error(what)
		{}
	};
}
