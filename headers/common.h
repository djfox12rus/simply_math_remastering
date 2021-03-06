
//Header ��� ����������� ����������� ��������� � ����������� ����� ����� � �������
#pragma once
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
//#include <wchar.h>
//#include <stdio.h>
//#include <math.h>
#include<Windows.h>
#include <cstdio>
#include <cstdlib>
#include <new>
#include"testing_and_logging.h"
#include "settings.h"


namespace SimpleMathApp {
	
	inline bool& NETEnvironment() {
		static bool NET = false;
		return NET;
	}

	namespace Core {
		//using namespace std;
		enum class flags : int16_t
		{
			//���� ���. ��������
			error = -10,
			math_dummy = -2,
			variable = -1,

			base = 0,

			real = 1,
			complex = 2,
			qwauterion = 3,

			constant_real = 10,
			constant_cmplx = 11,

			expression = 20,
			function = 21, //undef
			funct_defnd = 22,
			funct_arg_c = 23,
			funct_arg_v = 24,


			equation = 30,

			math_vector = 40,

			math_matrix = 50,

			operation = 60,

			addition = 61,
			subtraction = 62,
			multiplication = 63,
			division = 64,
			power = 65,
		};
		
		std::wstring GetStrForTypeFlag(flags _type);
	}
}


#endif // !COMMON_H
