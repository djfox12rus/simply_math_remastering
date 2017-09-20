#include "..\headers\data_list.h"

using namespace MemoryControl;
SimpleMathApp::data_list::_data_bit::_data_bit():left(), right(),in(),out()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(std::wstring &_in) : left(), right(), in(_in), out()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left):left(_left), right(), in(), out()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left, smart_ptr_weak<_data_bit> &_right) : left(_left), right(_right), in(), out()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left, smart_ptr_weak<_data_bit> &_right, std::wstring& _in) : left(_left), right(_right), in(_in), out()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(const _data_bit &_original) : left(_original.left), right(_original.right), in(_original.in), out(_original.out)
{
}

SimpleMathApp::data_list::_data_bit::~_data_bit()
{	
}

int SimpleMathApp::data_list::_data_bit::compare_in(std::wstring &_original)
{
	return in.compare(_original);
}

int SimpleMathApp::data_list::_data_bit::compare_out(std::wstring &_original)
{
	return out.compare(_original);
}

void SimpleMathApp::data_list::_data_bit::set_in(std::wstring& _str)
{
	in = _str;
}

std::wstring SimpleMathApp::data_list::_data_bit::get_in()
{
	return in;
}

void SimpleMathApp::data_list::_data_bit::set_out(std::wstring &_str)
{
	out = _str;
}

std::wstring SimpleMathApp::data_list::_data_bit::get_out()
{
	return out;
}

int SimpleMathApp::data_list::_data_bit::build_handler()
{
	return false;
}
