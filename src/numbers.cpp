#include "..\headers\constant_expressions.h"



SimpleMathApp::Core::real::real():num()
{
}

SimpleMathApp::Core::real::real(const long double& _num):num(_num)
{
}

SimpleMathApp::Core::real::real(const real &_real):num(_real.num)
{
}


SimpleMathApp::Core::real::~real()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::real::get_class_type()
{
	return flags::real;
}

size_t SimpleMathApp::Core::real::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::real::get_this()
{
	return this;
}

long double SimpleMathApp::Core::real::get_num()
{
	return num;
}

void SimpleMathApp::Core::real::set_num(long double& _num)
{
	num = _num;
}

uint16_t SimpleMathApp::Core::real::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) real(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::real::copy(math_obj * _original)
{
	if (_original->get_class_type() == flags::real) {//копирование ограничено элементами данного класса.
		this->num = _original->get_num();
		return this;
	}
	return nullptr;
}

std::wstring SimpleMathApp::Core::real::get_formal_entry()
{
	std::wostringstream strs;
	strs << num;
	std::wstring str = strs.str();
	return str;
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::real::process(tree_ptr &_tree)
{
	return this;
}


