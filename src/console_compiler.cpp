#include "..\headers\console_compiler.h"

int SimpleMathApp::ReadMathDatafromMudules()
{
	ModulesHandler mod_list;
	std::wifstream mod_file;
	bool no_files_read = true;	
	wchar_t wline[256];//TODO:длина строки 256 символов, сделать разумнее
	data_iterator iter = MathData()->before_begin();	
	uint32_t test_count = 0;	
		
	while (mod_list.GetNextModuleToRead(mod_file)) {
		no_files_read = false;			
		while (!mod_file.eof()) {
			mod_file.getline(wline, 256);			
			iter.emplace_after(std::wstring(wline));		
			iter++;			
		}
		test_count++;
		
	}
	if (SimpleMathApp::TestLog::TEST()) {
		data_iterator test_iter = MathData()->before_begin();		
		SimpleMathApp::TestLog::LOG_STREAM() << std::endl <<"From "<< test_count << " files following data have been read:" << std::endl;
		test_iter++;
		while (test_iter != MathData()->end()) {
			SimpleMathApp::TestLog::LOG_STREAM() << test_iter.get_in() << std::endl;
			test_iter++;
		}
	}

	return no_files_read ? -1 : 0;	
}

int SimpleMathApp::ProcessMathData()
{
	return 0;
}

int SimpleMathApp::ReturnResultintoFile()
{
	return 0;
}
