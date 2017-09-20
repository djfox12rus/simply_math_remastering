#include "../headers/testing_and_logging.h"



void SimpleMathApp::TestLog::SET_TEST() {
	bool & ref_test = SimpleMathApp::TestLog::TEST();
	ref_test = true;
	return;
}

void SimpleMathApp::TestLog::RESET_TEST()
{
	bool & ref_test = SimpleMathApp::TestLog::TEST();
	ref_test = false;
	return;
}

int SimpleMathApp::TestLog::InitLogFile() {
		
	std::string str;	

	SimpleMathApp::Settings::SettingsFileHandler file;	
	
	str = std::string(file.ReadPathToLogsFolder());
	str.append(SimpleMathApp::TestLog::get_current_date());
	str.append("__log.txt");		
	SimpleMathApp::TestLog::LOG_STREAM().open(str, std::fstream::out | std::fstream::app);

	if (LOG_STREAM().is_open()) {
		LOG_STREAM() << "Test started at "<< SimpleMathApp::TestLog::get_current_date()<<"."<<std::endl;
		return 0;
	}
	return -1;
}

void SimpleMathApp::TestLog::SaveLogFile()
{	
	LOG_STREAM() << "Test ended at " << SimpleMathApp::TestLog::get_current_date() << "." << std::endl;
	LOG_STREAM().close();
	//free(format);
}

const char * SimpleMathApp::TestLog::get_current_date()
{
	static char buffer[80];
	time_t seconds = time(NULL);
	tm timeinfo;
	localtime_s(&timeinfo, &seconds);
	char* format = "%d_%m_%Y__%Hh_%Mm_%Ss";
	strftime(buffer, 80, format, &timeinfo);
	return buffer;
}
