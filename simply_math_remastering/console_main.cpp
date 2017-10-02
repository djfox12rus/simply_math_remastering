
#define ___CONSOLE

#include "../headers/main_handler.h"


int main(/*std::string arg*/) {

	//if (arg == "test")
	//SimpleMathApp::TestLog::SET_TEST();
	SimpleMathApp::InitializeNETfalse();

	int not_good = SimpleMathApp::main_handler();	

	if (not_good) {
		std::cout << "Application returned " << not_good << ". Check logs." << std::endl;		
		std::string l;		
		std::cin>>l;
	}	
	
	return 0;
}
