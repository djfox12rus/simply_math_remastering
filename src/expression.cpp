#include "..\headers\constant_expressions.h"



SimpleMathApp::Core::expression::expression()
{
}

SimpleMathApp::Core::expression::expression(const leaf_ptr & _pl):point_left(_pl)
{
}

SimpleMathApp::Core::expression::expression(const expression &_exp) : point_left(_exp.point_left)
{
}


SimpleMathApp::Core::expression::~expression()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::expression::get_class_type()
{
	return flags::expression;
}

size_t SimpleMathApp::Core::expression::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::expression::get_this()
{
	return this;
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::expression::get_pl()
{
	return point_left;
}

void SimpleMathApp::Core::expression::set_pl(const leaf_ptr & _pointer)
{
	point_left = _pointer;
}

uint16_t SimpleMathApp::Core::expression::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) expression(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::expression::copy(math_obj * _original)
{
	if (_original->get_class_type() == flags::expression) {//копирование ограничено элементами данного класса.
		this->point_left = _original->get_pl();
		return this;
	}
	return nullptr;
}

std::wstring SimpleMathApp::Core::expression::get_formal_entry()
{
	if (point_left.is_nullptr())
		return std::wstring();
	std::wstring str = point_left->get_formal_entry();	
	return str;
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::expression::process(tree_ptr &_tree)
{
	if (point_left.is_nullptr())
		return nullptr;
	return point_left->process(_tree);
}



