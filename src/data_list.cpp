#include "..\headers\data_list.h"
using namespace MemoryControl;


SimpleMathApp::data_list::data_list():length(), begin_bit(_data_bit()), top_bit(),end_bit(_data_bit(begin_bit))
{
	begin_bit->right = end_bit;	
}


SimpleMathApp::data_list::~data_list()
{
	begin_bit.~smart_ptr_weak<_data_bit>();
	top_bit.~smart_ptr_weak<_data_bit>();
	end_bit.~smart_ptr_weak<_data_bit>();
}

size_t SimpleMathApp::data_list::size()
{
	return length;
}

void SimpleMathApp::data_list::push_back(std::wstring & _in)
{	
	smart_ptr_weak<_data_bit> before_end = end_bit->left;
	before_end->right = smart_ptr_weak<_data_bit>(_data_bit(before_end, end_bit, _in));
	end_bit->left = before_end->right;
	if (length == 0) {
		top_bit = end_bit->left;
	}
	length++;
	//TODO: build here!
}

void SimpleMathApp::data_list::push_left(/*Core::tree_ptr& pointer*/)
{
	smart_ptr_weak<_data_bit> after_begin = begin_bit->right;
	begin_bit->right = smart_ptr_weak<_data_bit>(_data_bit(begin_bit, after_begin));
	after_begin->left = begin_bit->right;
}

SimpleMathApp::data_list::iterator SimpleMathApp::data_list::before_begin()
{
	return iterator(begin_bit, this);
}

SimpleMathApp::data_list::iterator SimpleMathApp::data_list::top()
{
	if (top_bit.is_nullptr()) return iterator();
	return iterator(top_bit, this);
}

SimpleMathApp::data_list::iterator SimpleMathApp::data_list::end()
{
	return iterator(end_bit, this);
}

void SimpleMathApp::InitializeMathData()
{
	MathData();
}

void SimpleMathApp::DeleteMathData()
{
	data_list* temp = MathData();
	temp->~data_list();
}

void SimpleMathApp::ReinitializeMathData()
{
	DeleteMathData();
	data_list* temp = MathData();
	temp = new(temp) data_list();
}
