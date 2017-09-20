#pragma once
#ifndef MODULES_H
#define MODULES_H
#include"common.h"

namespace SimpleMathApp {

	class ModulesHandler
	{
	private:
		std::ifstream file_stream;
	public:
		ModulesHandler();
		~ModulesHandler();

		inline std::ifstream& MODULES_STREAM() {
			return file_stream;
		}

		bool GetNextModuleToRead(std::wifstream&);


	};


}

#endif // !MODULES_H




