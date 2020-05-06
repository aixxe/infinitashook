#pragma once

#include <ctime>
#include <iostream>

#ifdef __GNUC__
	// adapted from https://blog.galowicz.de/2016/02/20/short_file_macro/
	static constexpr const char* past_last_slash(const char* str, const char* last_slash)
	{
		if (*str == '\0')
			return last_slash;
		if (*str == '/' || *str == '\\')
			return past_last_slash(str + 1, str + 1);
		return past_last_slash(str + 1, last_slash);
	}

	static constexpr const char* past_last_slash(const char* str) 
	{ 
		return past_last_slash(str, str);
	}

	#define __SHORT_FILE__ ({constexpr const char* sf__ {past_last_slash(__FILE__)}; sf__;})
#else
	#define __SHORT_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1: __FILE__)
#endif

#define log_(format, type, ...) log::write(format, type, __SHORT_FILE__, __LINE__, ## __VA_ARGS__)

#define log_debug(format, ...) log_(format, "D", ## __VA_ARGS__)
#define log_info(format, ...)  log_(format, "I", ## __VA_ARGS__)
#define log_warn(format, ...)  log_(format, "W", ## __VA_ARGS__)
#define log_error(format, ...) log_(format, "E", ## __VA_ARGS__)

#ifdef NDEBUG
	#undef log_debug
	#define log_debug(format, ...)
#endif

inline const char* get_timestamp()
{
	time_t rawtime;
	struct tm timeinfo;

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	static char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

	return timestamp;
}

class log
{
	public:
		template <typename... Args> static void write(std::string format, const char* level, const char* file, int line, Args... args)
		{
			auto text = std::string("[%s] %s:%s:%d: " + format + "\n");
			printf(text.c_str(), get_timestamp(), level, file, line, args...);
		}
};