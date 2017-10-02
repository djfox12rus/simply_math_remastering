#include "..\headers\constant_expressions.h"



SimpleMathApp::Core::constant_real::constant_real():expression(), name(), num()
{
}

SimpleMathApp::Core::constant_real::constant_real(const leaf_ptr & _pl, const long double & _num, const std::wstring & _name):expression(_pl), name(_name),num(_num)
{
}

SimpleMathApp::Core::constant_real::constant_real(const constant_real &_cr) : expression(_cr.point_left), name(_cr.name), num(_cr.num)
{
}


SimpleMathApp::Core::constant_real::~constant_real()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::constant_real::get_class_type()
{
	return flags::constant_real;
}

size_t SimpleMathApp::Core::constant_real::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::constant_real::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::constant_real::get_name()
{
	return name;
}

void SimpleMathApp::Core::constant_real::set_name(const std::wstring & _name)
{
	name = _name;
}

long double SimpleMathApp::Core::constant_real::get_num()
{
	return num;
}

void SimpleMathApp::Core::constant_real::set_num(long double & _num)
{
	num = _num;
}

uint16_t SimpleMathApp::Core::constant_real::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) constant_real(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::constant_real::copy(math_obj * _original)
{
	flags type = _original->get_class_type();
	if (type == flags::constant_real || type == flags::expression || type == flags::real) {
		this->point_left = _original->get_pl();
		this->name = _original->get_name();
		this->num = _original->get_num();
		return this;
	}
	return nullptr;
}

std::wstring SimpleMathApp::Core::constant_real::get_formal_entry()
{
	if (point_left.is_nullptr()) {
		std::wostringstream strs;
		strs << num;
		std::wstring str = strs.str();
		return str;
	}	
	return expression::get_formal_entry();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::constant_real::process(tree_ptr &_tree)
{
	return nullptr;
}



