#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H
#include <fstream>
#include <string>

namespace SimpleMathApp {
	namespace Settings {

		class SettingsFileHandler {
		private:
			std::ifstream file_stream;
		public:
			SettingsFileHandler();
			~SettingsFileHandler();

			inline std::ifstream& SETTINGS_STREAM() {				
				return file_stream;
			}
			std::string ReadPathToLogsFolder();
			uint32_t ReadInitialMemorysize();
		};	
	}
}
#endif // !SETTINGS_H
