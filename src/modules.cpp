#include "..\headers\modules.h"



SimpleMathApp::ModulesHandler::ModulesHandler()
{

	file_stream.open("modules.ini", std::fstream::in);
}


SimpleMathApp::ModulesHandler::~ModulesHandler()
{
	file_stream.close();
}

bool SimpleMathApp::ModulesHandler::GetNextModuleToRead(std::wifstream& _out)
{
	char line[100];
	if (!file_stream.eof()) {
		file_stream.getline(line, 100);
	}
	else {
		return false;
	}
	if (_out.is_open()) {
		_out.close();
	}
	_out.open(line, std::fstream::in);
	return _out.is_open();	
}
