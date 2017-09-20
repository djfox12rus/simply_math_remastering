#include"../headers/settings.h"



std::string SimpleMathApp::Settings::SettingsFileHandler::ReadPathToLogsFolder()
{			
	if (!file_stream.is_open())
		return std::string();
	//file_stream.seekg(file_stream.beg);
	char line[100];
	bool found = false;
	while (!file_stream.eof()) {
		file_stream.getline(line,100);
		if (!strcmp(line, "[LogsPath]")) {			
			file_stream.getline(line, 100);
			found = true;
			break;
		}
	}	
	if (found) {
		std::string out = std::string(line);
		return out;
	}
	return std::string();
}

uint32_t SimpleMathApp::Settings::SettingsFileHandler::ReadInitialMemorysize()
{	
	if (!file_stream.is_open())
		return 32;

	file_stream.seekg(file_stream.beg);
	char line[100];
	bool found = false;
	while (!file_stream.eof()) {
		file_stream.getline(line, 100);
		if (!strcmp(line, "[MemoryControl]")) {
			file_stream.getline(line, 100);
			found = true;
			break;
		}
	}	
	if (found) {
		char* eq = strrchr(line, '=');
		eq++;
		uint32_t out = strtoul(eq, &eq, 0);
		return out;		
	}
	return 32;
}

SimpleMathApp::Settings::SettingsFileHandler::SettingsFileHandler():file_stream()
{
	file_stream.open("settings.ini", std::fstream::in);
	/*if (!SETTINGS_STREAM().is_open()) {
		throw std::runtime_error("Could not open file");
	}*/
}

SimpleMathApp::Settings::SettingsFileHandler::~SettingsFileHandler()
{
	file_stream.close();
}
