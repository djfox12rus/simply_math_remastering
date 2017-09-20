#pragma once
#ifndef TESTING_AND_LOGGING_H
#define TESTING_AND_LOGGING_H


#include <ctime>
#include<time.h>
#include <fstream>
//#include <cstdio>
#include <string>
#include "settings.h"

namespace SimpleMathApp {
	namespace TestLog {
		inline bool & TEST() {
			static bool _test = false;
			return _test;
		}

		void SET_TEST();

		void RESET_TEST();

		inline std::wofstream& LOG_STREAM() {
			static std::wofstream LOG;
			return LOG;
		}

		int InitLogFile();

		void SaveLogFile();

		const char* get_current_date();
		
	}
}
#endif // !TESTING_AND_LOGGING_H
