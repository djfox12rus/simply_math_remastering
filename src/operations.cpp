#include "..\headers\operations.h"



SimpleMathApp::Core::operations::operations()
{
}

SimpleMathApp::Core::operations::operations(const leaf_ptr & _pl):point_left(_pl), point_right(), point_collar()
{
}

SimpleMathApp::Core::operations::operations(const leaf_ptr & _pl, const leaf_ptr & _pr) : point_left(_pl), point_right(_pr), point_collar()
{
}

SimpleMathApp::Core::operations::operations(const leaf_ptr & _pl, const leaf_ptr & _pr, const leaf_ptr & _pc) : point_left(_pl), point_right(_pr), point_collar(_pc)
{
}


SimpleMathApp::Core::operations::~operations()
{
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::operations::get_pl()
{
	return point_left;
}

void SimpleMathApp::Core::operations::set_pl(const leaf_ptr & _pointer)
{
	point_left = _pointer;
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::operations::get_pr()
{
	return point_right;
}

void SimpleMathApp::Core::operations::set_pr(const leaf_ptr & _pointer)
{
	point_right = _pointer;
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::operations::get_pc()
{
	return point_collar;
}

void SimpleMathApp::Core::operations::set_pc(const leaf_ptr & _pointer)
{
	point_collar = _pointer;
}

bool SimpleMathApp::Core::operations::define_operation(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this)
{
	if (_high.is_nullptr() && _low.is_nullptr()) {
		//err
		return false;
	}
	//если это первая операция в выражении
	else if (_high == _low) {
		point_left = _low;
		point_collar = _obj;
		_obj->set_pl(_this);
	}
	//если была какая-либо операция до этого
	else {
		//если приоритет предыдущей обработанной операции !МЕНЬШЕ! приоритета текущей
		if (_high->get_priority() < this->get_priority()) {
			point_left = _low;
			_high->set_pr(_this);
			point_collar = _high;
		}
		else {
			//leaf_ptr temp;
			leaf_ptr top = _obj->get_pl();
			flags type = top->get_class_type();
			if (type != flags::addition&&type != flags::subtraction&&type != flags::multiplication&&type != flags::division&&type != flags::power)
				return false; //err					
			if (top->get_priority() >= this->get_priority()) {
				point_left = top;//много преобразований указателей. Проверить правильность работы
				point_collar = _obj;
				_obj->set_pl(_this);
				top->set_pr(_this);
			}
			else {
				top = ((operations*)_high.get_ptr_unsafe())->prioritize(this->get_priority());//операция с нужным приоритетом
				if (top.is_nullptr()) {
					return false;
				}
				point_left = top->get_pr();
				point_collar = top;
				point_collar->set_pr(_this);
				point_left->set_pc(_this);
			}
		}
	}
	return true;
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::operations::prioritize(int _priority)
{	

	return (point_collar->get_priority() <= _priority) ? point_collar : ((operations*)point_collar.get_ptr_unsafe())->prioritize_rec(_priority);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::operations::copy(math_obj * _original)
{
	flags type = _original->get_class_type();
	if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power) {
		this->point_left = _original->get_pl();
		this->point_collar = _original->get_pc();
		this->point_right = _original->get_pr();
		return this;
	}
	return nullptr;
}

std::wstring SimpleMathApp::Core::operations::get_formal_entry()
{
	if (point_left.is_nullptr() || point_right.is_nullptr())
		return std::wstring();
	flags type = point_left->get_class_type();
	std::wstring str;
	if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power || type == flags::real) {
		str.append(point_left->get_formal_entry());
	}
	else {
		str.append(L"(");
		str.append(point_left->get_formal_entry());
		str.append(L")");
	}	
	str.append(this->get_name());
	type = point_right->get_class_type();
	if (type == flags::addition || type == flags::subtraction || type == flags::multiplication || type == flags::division || type == flags::power || type == flags::real) {
		str.append(point_right->get_formal_entry());
	}
	else {
		str.append(L"(");
		str.append(point_right->get_formal_entry());
		str.append(L")");
	}
	return str;
}

SimpleMathApp::Core::leaf_ptr SimpleMathApp::Core::operations::prioritize_rec(int & _p)
{	
	//возможно требуется проверка типа дерева перед приведением указателя math_obj* к operations*, хотя по построению никаких неопределённых ситуаций не должно возникать
	return (point_collar->get_priority() <= _p)? point_collar: ((operations*)point_collar.get_ptr_unsafe())->prioritize_rec(_p);
}


SimpleMathApp::Core::addition::addition()
{
}

SimpleMathApp::Core::addition::addition(const leaf_ptr & _pl):operations(_pl)
{
}

SimpleMathApp::Core::addition::addition(const leaf_ptr & _pl, const leaf_ptr & _pr):operations (_pl, _pr)
{
}

SimpleMathApp::Core::addition::addition(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this) throw(...)
{
	if (!define_operation(_high, _low, _obj, _this)) {
		//throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
		throw std::runtime_error("OPERATIONS_CONSTRUCT_FAILED");
	}
}

SimpleMathApp::Core::addition::addition(const addition &_obj):operations(_obj.point_left, _obj.point_right, _obj.point_collar)
{
}


SimpleMathApp::Core::addition::~addition()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::addition::get_class_type()
{
	return flags::addition;
}

size_t SimpleMathApp::Core::addition::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::addition::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::addition::get_name()
{
	return L"+";
}



int SimpleMathApp::Core::addition::get_priority()
{
	return 1;
}

uint16_t SimpleMathApp::Core::addition::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) addition(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::addition::process(tree_ptr &_tree)
{
	if (point_left.is_nullptr()|| point_right.is_nullptr())
		return nullptr;
	math_obj* left = point_left->process(_tree);
	if (left->get_class_type() == flags::real) {

	}
	

	return nullptr;
}






SimpleMathApp::Core::subtraction::subtraction()
{
}

SimpleMathApp::Core::subtraction::subtraction(const leaf_ptr & _pl) :operations(_pl)
{
}

SimpleMathApp::Core::subtraction::subtraction(const leaf_ptr & _pl, const leaf_ptr & _pr) :operations(_pl, _pr)
{
}

SimpleMathApp::Core::subtraction::subtraction(const leaf_ptr & _pl, const leaf_ptr & _pr, const leaf_ptr & _pc) : operations(_pl, _pr,_pc)
{
}

SimpleMathApp::Core::subtraction::subtraction(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this) throw(...)
{
	if (!define_operation(_high, _low, _obj, _this)) {
		//throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
		throw std::runtime_error("OPERATIONS_CONSTRUCT_FAILED");
	}
}

SimpleMathApp::Core::subtraction::subtraction(const subtraction &_obj) :operations(_obj.point_left, _obj.point_right, _obj.point_collar)
{
}


SimpleMathApp::Core::subtraction::~subtraction()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::subtraction::get_class_type()
{
	return flags::subtraction;
}

size_t SimpleMathApp::Core::subtraction::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::subtraction::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::subtraction::get_name()
{
	return L"-";
}



int SimpleMathApp::Core::subtraction::get_priority()
{
	return 1;
}

uint16_t SimpleMathApp::Core::subtraction::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) subtraction(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}






SimpleMathApp::Core::multiplication::multiplication()
{
}

SimpleMathApp::Core::multiplication::multiplication(const leaf_ptr & _pl) :operations(_pl)
{
}

SimpleMathApp::Core::multiplication::multiplication(const leaf_ptr & _pl, const leaf_ptr & _pr) :operations(_pl, _pr)
{
}

SimpleMathApp::Core::multiplication::multiplication(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this) throw(...)
{
	if (!define_operation(_high, _low, _obj, _this)) {
		//throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
		throw std::runtime_error("OPERATIONS_CONSTRUCT_FAILED");
	}
}

SimpleMathApp::Core::multiplication::multiplication(const multiplication &_obj) :operations(_obj.point_left, _obj.point_right, _obj.point_collar)
{
}


SimpleMathApp::Core::multiplication::~multiplication()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::multiplication::get_class_type()
{
	return flags::multiplication;
}

size_t SimpleMathApp::Core::multiplication::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::multiplication::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::multiplication::get_name()
{
	return L"*";
}



int SimpleMathApp::Core::multiplication::get_priority()
{
	return 2;
}

uint16_t SimpleMathApp::Core::multiplication::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) multiplication(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}




SimpleMathApp::Core::division::division()
{
}

SimpleMathApp::Core::division::division(const leaf_ptr & _pl) :operations(_pl)
{
}

SimpleMathApp::Core::division::division(const leaf_ptr & _pl, const leaf_ptr & _pr) :operations(_pl, _pr)
{
}

SimpleMathApp::Core::division::division(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this) throw(...)
{
	if (!define_operation(_high, _low, _obj, _this)) {
		//throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
		throw std::runtime_error("OPERATIONS_CONSTRUCT_FAILED");
	}
}

SimpleMathApp::Core::division::division(const division &_obj) :operations(_obj.point_left, _obj.point_right, _obj.point_collar)
{
}


SimpleMathApp::Core::division::~division()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::division::get_class_type()
{
	return flags::division;
}

size_t SimpleMathApp::Core::division::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::division::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::division::get_name()
{
	return L"/";
}



int SimpleMathApp::Core::division::get_priority()
{
	return 2;
}

uint16_t SimpleMathApp::Core::division::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) division(*this);
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}




SimpleMathApp::Core::power::power()
{
}

SimpleMathApp::Core::power::power(const leaf_ptr & _pl) :operations(_pl)
{
}

SimpleMathApp::Core::power::power(const leaf_ptr & _pl, const leaf_ptr & _pr) :operations(_pl, _pr)
{
}

SimpleMathApp::Core::power::power(const leaf_ptr & _high, const leaf_ptr & _low, const leaf_ptr & _obj, const leaf_ptr & _this) throw(...)
{
	if (!define_operation(_high, _low, _obj, _this)) {
		//throw ProjectError::ErrorCode::OPERATIONS_CONSTRUCT_FAILED;
		throw std::runtime_error("OPERATIONS_CONSTRUCT_FAILED");
	}
}

SimpleMathApp::Core::power::power(const power &_obj) :operations(_obj.point_left, _obj.point_right, _obj.point_collar)
{
}


SimpleMathApp::Core::power::~power()
{
}

SimpleMathApp::Core::flags SimpleMathApp::Core::power::get_class_type()
{
	return flags::power;
}

size_t SimpleMathApp::Core::power::get_sizeof()
{
	return sizeof(*this);
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::power::get_this()
{
	return this;
}

std::wstring SimpleMathApp::Core::power::get_name()
{
	return L"^";
}


int SimpleMathApp::Core::power::get_priority()
{
	return 3;
}

uint16_t SimpleMathApp::Core::power::place_copy_to(void * _ptr)
{
	math_obj *place = new(_ptr) power(*this);	
	return (uint16_t)((uint8_t*)place->get_this_void() - (uint8_t*)_ptr);
}


