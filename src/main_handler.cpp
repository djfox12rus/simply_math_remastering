
#include "../headers/main_handler.h"





int SimpleMathApp::testing_ground()
{
	using namespace MemoryControl;
	/*smart_ptr_weak<double> ptr1;
	ptr1.place(7.54);
	smart_ptr_strong<double> ptr2 = smart_ptr_strong<double>(4.658);
	smart_ptr_weak<double>ptr3 = smart_ptr_weak<double>(ptr2);*/	

	/*std::wstring a = L"testing1";
	std::wstring b = L"testing2";
	std::wstring d = L"test";


	data_list data;
	data.push_back(a);
	data.push_back(b);
	TODO:заставить работать итераторы!!!
	data_iterator it = data.top();
	data_iterator it2 = it.emplace_after(d);
	std::wstring str = it.get_in();
	str = it2.get_in();*/

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
		SimpleMathApp::TestLog::SaveLogFile();
		return -1;
	}
	catch (...) {
		SimpleMathApp::TestLog::SaveLogFile();
		return -1;
	}	

	try {
		SimpleMathApp::InitializeMathData();//список данных
	}
	catch (...) {
		SimpleMathApp::TestLog::SaveLogFile();
		return -1;
	}

	if (NETEnvironment()) {
		//вызов и работа с формой .NET
	}
	else {//работа в режиме "компилятора"
		if (SimpleMathApp::ReadMathDatafromMudules()) {
			std::cout <<"Could not read data from files. Check logs." << std::endl;
			SimpleMathApp::TestLog::SaveLogFile();
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


