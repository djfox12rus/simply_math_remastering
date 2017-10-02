
#include "../headers/main_handler.h"





int SimpleMathApp::testing_ground()
{
	using namespace MemoryControl;
	std::wstring str = L"1";
	MathData()->push_back(str);
	str = L"2";
	MathData()->push_back(str);
	str = L"3";
	MathData()->push_back(str);
	data_iterator it = MathData()->before_begin();
	//it++;
	MathData()->delete_after(it);
	size_t check = MathData()->size();

	MathData()->push_back(str);
	MathData()->push_back(str);
	it = MathData()->top();

	check = MathData()->size();

	return 0;
}

int SimpleMathApp::main_handler() {

	if (SimpleMathApp::TestLog::TEST()) {
		if (SimpleMathApp::TestLog::InitLogFile()) {//открывается лог-файл
			std::cout << "Could not create log file. Please check [LogsPath] section in settings.ini and configure the available path for saving logs." << std::endl;
			std::cout << "Could not continue with testing option. Test variable resets. The program will continue in regular mode in 5 seconds" << std::endl;
#ifdef _WINDOWS_
			Sleep(5000);
#endif
		}
	}
	try {
		MemoryControl::InitializeMemPool();//выделяется пул памяти
	}
	catch (std::bad_alloc err) {
		std::cout << "Bad alloc" << std::endl;
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::SaveLogFile();
		}
		return -1;
	}
	catch (...) {
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::SaveLogFile();
		}
		return -1;
	}	

	try {
		SimpleMathApp::InitializeMathData();//список данных
	}
	catch (...) {
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::SaveLogFile();
		}
		return -1;
	}


	//SimpleMathApp::testing_ground();
	if (NETEnvironment()) {
		//вызов и работа с формой .NET
	}
	else {//работа в режиме "компилятора"
		if (SimpleMathApp::ReadMathDatafromMudules()) {
			std::cout <<"Could not read data from files. Check logs." << std::endl;
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::SaveLogFile();
			}
			return -1;
		}
		if (SimpleMathApp::ProcessMathData()) {
			std::cout << "Could not read data from files. Check logs." << std::endl;
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::SaveLogFile();
			}
			return -1;
		}

	}



	SimpleMathApp::DeleteMathData();
	MemoryControl::DestMemPool();
	if (SimpleMathApp::TestLog::TEST()) {
		SimpleMathApp::TestLog::SaveLogFile();
	}
	return 0;
}

void SimpleMathApp::InitializeNETfalse()
{
	NETEnvironment();
}

void SimpleMathApp::SET_NET_ON()
{
	bool& temp = NETEnvironment();
	temp = true;
}


