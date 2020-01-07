#include "Debug.h"




inline void Debug::SetPattern(std::string pattern)
{
	spdlog::set_pattern(pattern);
}

inline void Debug::SetDebugState(bool newState)
{
	debugOn = newState;
}
