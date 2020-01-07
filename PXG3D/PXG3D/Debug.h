#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <iostream>
#include <string>


enum class Verbosity
{
	Default,
	Info,
	Debug,
	Warning,
	Error
};

static class Debug
{
public:
	//static Verbosity DefaultVerbosity;

	template<typename ...Args>
	static void Log(Verbosity verbosity, std::string str, const Args & ...args)
	{
		Verbosity verbosityUsed = verbosity;

		switch (verbosityUsed)
		{
		case Verbosity::Info:
			spdlog::set_level(spdlog::level::info);
			spdlog::info(str, args...);
			break;
		case Verbosity::Debug:
			spdlog::set_level(spdlog::level::debug);
			spdlog::debug(str,args...);
			break;
		case Verbosity::Warning:
			spdlog::set_level(spdlog::level::warn);
			spdlog::warn(str, args...);
			break;
		case Verbosity::Error:
			spdlog::set_level(spdlog::level::err);
			spdlog::error(str, args...);
			break;

		default:
			break;
		}

	};
	

	static void SetPattern(std::string pattern);

	static void SetDebugState(bool newState);

private:

	static bool debugOn;
	

};


