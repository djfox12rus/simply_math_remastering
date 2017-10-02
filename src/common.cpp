#include "../headers/common.h"

std::wstring SimpleMathApp::Core::GetStrForTypeFlag(flags _type) {
	switch (_type)
	{
	case SimpleMathApp::Core::flags::error:
		return L"error";
		break;
	case SimpleMathApp::Core::flags::math_dummy:
		return L"math_dummy";
		break;
	case SimpleMathApp::Core::flags::variable:
		return L"variable";
		break;
	case SimpleMathApp::Core::flags::base:
		return L"base";
		break;
	case SimpleMathApp::Core::flags::real:
		return L"real";
		break;
	case SimpleMathApp::Core::flags::complex:
		return L"complex";
		break;
	case SimpleMathApp::Core::flags::qwauterion:
		return L"qwauterion";
		break;
	case SimpleMathApp::Core::flags::constant_real:
		return L"constant_real";
		break;
	case SimpleMathApp::Core::flags::constant_cmplx:
		return L"constant_cmplx";
		break;
	case SimpleMathApp::Core::flags::expression:
		return L"expression";
		break;
	case SimpleMathApp::Core::flags::function:
		return L"function";
		break;
	case SimpleMathApp::Core::flags::funct_defnd:
		return L"funct_defnd";
		break;
	case SimpleMathApp::Core::flags::funct_arg_c:
		return L"funct_arg_c";
		break;
	case SimpleMathApp::Core::flags::funct_arg_v:
		return L"funct_arg_v";
		break;
	case SimpleMathApp::Core::flags::equation:
		return L"equation";
		break;
	case SimpleMathApp::Core::flags::math_vector:
		return L"math_vector";
		break;
	case SimpleMathApp::Core::flags::math_matrix:
		return L"math_matrix";
		break;
	case SimpleMathApp::Core::flags::operation:
		return L"operation";
		break;
	case SimpleMathApp::Core::flags::addition:
		return L"addition";
		break;
	case SimpleMathApp::Core::flags::subtraction:
		return L"subtraction";
		break;
	case SimpleMathApp::Core::flags::multiplication:
		return L"multiplication";
		break;
	case SimpleMathApp::Core::flags::division:
		return L"division";
		break;
	case SimpleMathApp::Core::flags::power:
		return L"power";
		break;
	default:
		return L"unknown";
		break;
	}
}