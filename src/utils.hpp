#pragma once

#include <sstream>

inline std::string to_hex(uint64_t value)
{
	std::stringstream ss;
	ss << "0x" << std::hex << value;
	return ss.str();
}
