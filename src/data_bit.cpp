#include "..\headers\data_list.h"

using namespace MemoryControl;

SimpleMathApp::data_list::_data_bit::_data_bit():left(), right(),in(),out(), object()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(std::wstring &_in) : left(), right(), in(_in), out(), object()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left):left(_left), right(), in(), out(), object()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left, smart_ptr_weak<_data_bit> &_right) : left(_left), right(_right), in(), out(), object()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left, smart_ptr_weak<_data_bit> &_right, std::wstring& _in) : left(_left), right(_right), in(_in), out(), object()
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(smart_ptr_weak<_data_bit> &_left, smart_ptr_weak<_data_bit> &_right, std::wstring& _in, Core::tree_ptr &_obj) : left(_left), right(_right), in(_in), out(), object(_obj)
{
}

SimpleMathApp::data_list::_data_bit::_data_bit(const _data_bit &_original) : left(_original.left), right(_original.right), in(_original.in), out(_original.out)
{
}

SimpleMathApp::data_list::_data_bit::~_data_bit()
{	
	int a = 1;
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
	/*if (!IO::VerifyInput(&in[0])) {
		ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
		ProjectError::GetProjectLastError(err);
		out.append(err->GetErrorWStr());
		return -1;
	}*/
	using namespace Core;
	expression temp = expression();
	expression* temp_ptr = &temp;
	object = tree_ptr(temp_ptr, 5);
	try {
		this->build_iternal(&in[0], &in[in.length()] - 1);
		//this->object.srink_to_last_ref();
	}
	/*catch (ProjectError::ErrorCode error_caught) {
		ProjectError::SetProjectLastError(error_caught);
		ProjectError::_ErrorPresent* err = new ProjectError::_ErrorPresent();
		ProjectError::GetProjectLastError(err);
		out.append(err->GetErrorWStr());
		return -1;
	}*/
	catch (const std::bad_exception& e)
	{
		wchar_t error_str[256];		
		size_t outSize;
		mbstowcs_s(&outSize, error_str, 256, e.what(), 255);
		out.append(error_str);
	}
	catch (...) {
		out.append(L"Unresolved error.");
		return -1;
	}
	return 0;
}

std::wstring SimpleMathApp::data_list::_data_bit::get_obj_formal_entry()
{
	return object->get_formal_entry();
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::data_list::_data_bit::build_iternal(wchar_t * str_begin, wchar_t * str_end)
{
	using namespace Core;
	wchar_t* temp = wcschr(str_begin, L'=');
	leaf_ptr temp_ptr;
	expression temp_exp;
	if (temp == nullptr) {//если не найдено знака равно в строке - оконечный случай - то что будет построено по заданному куску строки должно уйти наверх.				
		return build_tree(str_begin, str_end, object.push_obj(&temp_exp, max_size));
	}	

	std::wstring str_left;
	str_left.assign(str_begin, temp);
	leaf_ptr left = build_tree(&str_left[0], &str_left[str_left.length() - 1], object);
	if (temp == str_end) {//строка оканчивается на '='		
		return left;
	}

	std::wstring str_right;
	str_right.assign(temp + 1, str_end + 1);
	temp_exp = expression();	
	leaf_ptr right = build_tree(&str_right[0], &str_right[str_right.length() - 1], object.push_obj(&temp_exp, max_size));
	//TODO:далее самое веселье)
	return Core::leaf_ptr();
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::data_list::_data_bit::build_tree(wchar_t * _str, wchar_t * _end, Core::leaf_ptr _current_obj)
{
	using namespace Core;
	unsigned int count, comma, iter;
	int count_var;
	leaf_ptr last_op;//последн¤¤ записанна¤ операци¤
	leaf_ptr last_obj; //последн¤¤ записанна¤ константа/переменна¤/ф-ци¤/выражение
	leaf_ptr temp_pointer;
	leaf_ptr multiple_var;
	math_obj* unsafe_ptr;

	wchar_t* temp/*, *temp_2*/;
	//std::wstring temp_str, name_str;
	double buf;
	wchar_t* s_iter = _str;
	while (s_iter <= _end) {

		if (*s_iter >= '0'&& *s_iter <= '9' || *s_iter == '.') {
			// если данное число первое в записи выражени¤
			if (last_op.is_nullptr() && last_obj.is_nullptr()) {
				//создание элемента класса и запись числа
				temp_pointer = leaf_ptr(object, &real(wcstod(s_iter, &s_iter)));
				_current_obj->set_pl(temp_pointer);
				//оба указател¤ -> на число, тебуетс¤ дл¤ проверки услови¤ при записи операции
				last_obj = object->get_pl();
				last_op = last_obj;
			}
			// если данное число не первое в записи (была кака¤-либо операци¤)
			else {
				//создание элемента класса и запись числа
				last_op->set_pr(leaf_ptr(object, &real(wcstod(s_iter, &s_iter))));
				last_obj = last_op->get_pr();
			}
			//temp = wcspbrk(s_iter, L",");
			temp = wcspbrk(s_iter, L"(");//если сразу после числа стоит открывающа¤ скобка
			if (s_iter == temp)
			{//добавл¤етс¤ умножение
				temp_pointer = leaf_ptr(object, &multiplication());
				temp_pointer->copy(&multiplication(last_op, last_obj, _current_obj, temp_pointer));
				last_op = temp_pointer;
			}
		}
		else if (*s_iter == '+') {
			temp_pointer = leaf_ptr(object, &addition());
			temp_pointer->copy(&addition(last_op, last_obj, _current_obj, temp_pointer));
			last_op = temp_pointer;
			s_iter++;
			temp = wcspbrk(s_iter, L")+-*^/=,");
			if (s_iter == temp) {
				throw std::runtime_error("UNEXPECTED_OPERATION");
				//throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
			}
		}
		else if (*s_iter == '-') {
			if (last_op.is_nullptr() && last_obj.is_nullptr()) {//если в начале строки находитс¤ минус
				leaf_ptr null_ptr = leaf_ptr();
				last_obj = leaf_ptr(object, &real());
				temp_pointer = leaf_ptr(object, &subtraction(last_obj, null_ptr, _current_obj));
				_current_obj->set_pl(temp_pointer);
			}
			else {
				temp_pointer = leaf_ptr(object, &subtraction());
				temp_pointer->copy(&subtraction(last_op, last_obj, _current_obj, temp_pointer));
			}
			last_op = temp_pointer;
			s_iter++;
			temp = wcspbrk(s_iter, L")+-*^/=,");
			if (s_iter == temp) {
				throw std::runtime_error("UNEXPECTED_OPERATION");
				//throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
			}
		}
		else if (*s_iter == '*') {
			temp_pointer = leaf_ptr(object, &multiplication());
			temp_pointer->copy(&multiplication(last_op, last_obj, _current_obj, temp_pointer));
			last_op = temp_pointer;
			s_iter++;
			temp = wcspbrk(s_iter, L")+-*^/=,");
			if (s_iter == temp) {
				throw std::runtime_error("UNEXPECTED_OPERATION");
				//throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
			}
		}
		else if (*s_iter == '/') {
			temp_pointer = leaf_ptr(object, &division());
			temp_pointer->copy(&division(last_op, last_obj, _current_obj, temp_pointer));
			last_op = temp_pointer;
			s_iter++;
			temp = wcspbrk(s_iter, L")+-*^/=,");
			if (s_iter == temp) {
				throw std::runtime_error("UNEXPECTED_OPERATION");
				//throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
			}
		}
		else if (*s_iter == '^') {
			temp_pointer = leaf_ptr(object, &power());
			temp_pointer->copy(&power(last_op, last_obj, _current_obj, temp_pointer));
			last_op = temp_pointer;
			s_iter++;
			temp = wcspbrk(s_iter, L")+-*^/=,");
			if (s_iter == temp) {
				throw std::runtime_error("UNEXPECTED_OPERATION");
				//throw ProjectError::ErrorCode::UNEXPECTED_OPERATION;
			}
		}
		else if (*s_iter == '(') {

		}
		else if (*s_iter == '[') {

		}
		else {//другие символы

		}
	}

	return _current_obj;
}
