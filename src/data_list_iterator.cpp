#include "..\headers\data_list.h"
using namespace MemoryControl;
//using smart_ptr_weak<SimpleMathApp::data_list::_data_bit>;


SimpleMathApp::data_list::iterator::iterator():letter(), this_data_list(nullptr)
{
}

SimpleMathApp::data_list::iterator::iterator(smart_ptr_weak<_data_bit> &_letter, data_list* _this_list):letter(_letter), this_data_list(_this_list)
{
}

SimpleMathApp::data_list::iterator::iterator(const iterator &_original):letter(_original.letter), this_data_list(_original.this_data_list)
{
}

SimpleMathApp::data_list::iterator::iterator(const iterator &&_original) : letter(_original.letter), this_data_list(_original.this_data_list)
{
}

SimpleMathApp::data_list::iterator::~iterator()
{
	/*letter.~smart_ptr_weak<_data_bit>();*/
}

void SimpleMathApp::data_list::iterator::set_in(std::wstring &_str)
{
	if(this->letter.is_nullptr())
		this->letter->in = _str;
}

std::wstring SimpleMathApp::data_list::iterator::get_in()
{
	return this->letter.is_nullptr()? std::wstring(): this->letter->in;
}

void SimpleMathApp::data_list::iterator::set_out(std::wstring &_str)
{
	if (this->letter.is_nullptr())
		this->letter->out = _str;
}

std::wstring SimpleMathApp::data_list::iterator::get_out()
{
	return this->letter.is_nullptr() ? std::wstring() : this->letter->out;
}

SimpleMathApp::Core::tree_ptr SimpleMathApp::data_list::iterator::get_obj()
{
	return letter->object;
}

void SimpleMathApp::data_list::iterator::set_obj(Core::math_obj * _obj)
{
	letter->object = Core::tree_ptr(_obj);
}

int SimpleMathApp::data_list::iterator::compare_in(std::wstring & _original)
{
	return (this->letter.is_nullptr())? -1 : this->letter->compare_in(_original);
}

int SimpleMathApp::data_list::iterator::compare_out(std::wstring & _original)
{
	return (this->letter.is_nullptr()) ? -1 : this->letter->compare_out(_original);
}

SimpleMathApp::data_list::iterator  SimpleMathApp::data_list::iterator::emplace_after(std::wstring &_str)
{
	if (this->letter.is_nullptr()) return *this;
	smart_ptr_weak<_data_bit> next_bit = letter->right;
	letter->right = smart_ptr_weak<_data_bit>(_data_bit(letter, next_bit, _str));
	next_bit->left = letter->right;
	if (this_data_list->length == 0) {
		this_data_list->top_bit = next_bit->left;
	}
	this_data_list->length++;	
	letter->right->build_handler();
	return iterator(letter->right, this->this_data_list);
}

int SimpleMathApp::data_list::iterator::build()
{
	return (this->letter.is_nullptr()) ? -1 : letter->build_handler();
}

SimpleMathApp::data_list::iterator & SimpleMathApp::data_list::iterator::operator=(const iterator & _right)
{
	if (this == &_right) return *this;//проверка равенства по значению
	this->letter = _right.letter;
	this->this_data_list = _right.this_data_list;
	return *this;
}

SimpleMathApp::data_list::iterator & SimpleMathApp::data_list::iterator::operator++(int)
{
	if (this->letter.is_nullptr())return *this;
	this->letter = this->letter->right;
	return *this;
}

SimpleMathApp::data_list::iterator & SimpleMathApp::data_list::iterator::operator++()
{
	if (this->letter.is_nullptr())return *this;
	this->letter = this->letter->right;
	return *this;
}

SimpleMathApp::data_list::iterator & SimpleMathApp::data_list::iterator::operator--(int)
{
	if (this->letter.is_nullptr())return *this;
	this->letter = this->letter->left;
	return *this;
}

SimpleMathApp::data_list::iterator & SimpleMathApp::data_list::iterator::operator--()
{
	if (this->letter.is_nullptr())return *this;
	this->letter = this->letter->left;
	return *this;
}

SimpleMathApp::data_list::_data_bit * SimpleMathApp::data_list::iterator::operator->()
{
	return letter.operator->();
}

SimpleMathApp::data_list::_data_bit * SimpleMathApp::data_list::iterator::operator->() const
{
	return letter.operator->();
}


bool SimpleMathApp::operator== (const SimpleMathApp::data_list::iterator& _left, const SimpleMathApp::data_list::iterator& _right) {	
	//return (&_left == &_right)||(_left.this_data_list == _right.this_data_list && _left.letter.compare(&_right.letter));
	if (&_left == &_right) return true;
	if (_left.this_data_list != _right.this_data_list)return false;
	smart_ptr_weak<SimpleMathApp::data_list::_data_bit> left = _left.letter;
	smart_ptr_weak<SimpleMathApp::data_list::_data_bit> right = _right.letter;
	return left.compare(&right);
	return false;
}

bool SimpleMathApp::operator!=(const SimpleMathApp::data_list::iterator & _left, const SimpleMathApp::data_list::iterator & _right)
{
	return !(_left == _right);
}


