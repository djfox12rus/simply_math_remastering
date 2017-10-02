#include "..\headers\math_obj.h"
//размер выделяемой памяти под вершину дерева (с запасом). Полный объём памяти под всё дерево выделяется кратно этому числу

using namespace MemoryControl;

SimpleMathApp::Core::tree_ptr::tree_ptr():s_ref(nullptr), last_ref(), virtual_ref()
{
}

SimpleMathApp::Core::tree_ptr::tree_ptr(math_obj *_obj, size_t _size): s_ref(nullptr), last_ref(max_size), virtual_ref()
{
	if (SimpleMathApp::TestLog::TEST()) {	
		SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "tree_ptr constructor with arg for " << GetStrForTypeFlag(_obj->get_class_type()) << ", size: "<< _size;
	}
	s_ref = MemPool()->allocate_mem(max_size, _size);
	virtual_ref = _obj->place_copy_to(this->get_real_ptr());

}

SimpleMathApp::Core::tree_ptr::tree_ptr(const tree_ptr &_tree):s_ref(_tree.s_ref), last_ref(_tree.last_ref), virtual_ref(_tree.virtual_ref)
{
	if (!this->is_nullptr()) {
		this->s_ref->ref_count()++;
	}
}


SimpleMathApp::Core::tree_ptr::~tree_ptr()
{
	if (!this->is_nullptr()) {
		if (s_ref->ref_count() > 1) {
			s_ref->ref_count()--;
		}
		else if (s_ref->ref_count() == 1) {
			math_obj* ptr = this->get_ptr_unsafe();
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "tree_ptr destructor for " << GetStrForTypeFlag(ptr->get_class_type()) << ". Shift is: " << s_ref->shift() << ". Real pointer is: " << get_real_ptr() << ". Virtual pointer is: " << ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
			}
			s_ref->ref_count()--;
			ptr->~math_obj();			
		}
	}
}

void * SimpleMathApp::Core::tree_ptr::get_real_ptr(uint32_t _plus_ref)
{
	return (this->is_nullptr()) ? nullptr : ((int8_t*)MemPool()->get_block() + s_ref->shift() + _plus_ref);
}

void * SimpleMathApp::Core::tree_ptr::get_real_ptr(uint32_t _plus_ref) const
{
	return (this->is_nullptr()) ? nullptr : ((int8_t*)MemPool()->get_block() + s_ref->shift() + _plus_ref);
}

bool SimpleMathApp::Core::tree_ptr::check_if_can_continue(size_t size_of_type, size_t _new_size_of_arr)
{
	ref_iterator this_ref = MemPool()->get_iterator_before(s_ref);
	this_ref++;
	ref_iterator next_ref = this_ref;
	next_ref++;
	ref_iterator next_next_ref = next_ref;
	next_next_ref++;
	if (!next_ref->ref_count()) {//далее записывать имеет смысл только если на следующую запись ничего не указывает
		if (next_next_ref != MemPool()->get_ref_table().end()) {
			if (s_ref->shift() + _new_size_of_arr*size_of_type < next_next_ref->shift()) {
				next_ref->shift() = s_ref->shift() + _new_size_of_arr*size_of_type;
				return true;
			}
			if (s_ref->shift() + _new_size_of_arr*size_of_type == next_next_ref->shift()) {
				next_ref = MemPool()->get_ref_table().erase_after(this_ref);
				return true;
			}
		}
		else {//next_next_ref == MemPool().reference_table.end()
			if ((int8_t*)MemPool()->get_block() + (s_ref->shift() + _new_size_of_arr*size_of_type) < MemPool()->get_block_end()) {//есть место в блоке памяти
				next_ref->shift() = s_ref->shift() + _new_size_of_arr*size_of_type;
				return true;
			}
		}
	}
	return false;
}

void SimpleMathApp::Core::tree_ptr::alloc(size_t _size_of_arr)
{
	if (s_ref != nullptr) return;
	//если память уже выделена - делать ничего не надо.
	s_ref = MemPool()->allocate_mem(max_size, _size_of_arr);
}

void SimpleMathApp::Core::tree_ptr::realloc(size_t _new_size_of_arr)
{
	if (s_ref == nullptr) return alloc(_new_size_of_arr);
	ref_iterator this_ref = MemPool()->get_iterator_before(s_ref);
	this_ref++;
	ref_iterator next_ref = this_ref;
	next_ref++;
	size_t num_of_bytes = next_ref->shift() - s_ref->shift();
	if (_new_size_of_arr*max_size < num_of_bytes)return;
	if (check_if_can_continue(max_size, _new_size_of_arr))	return;
	_ref *new_ref = MemPool()->relocate_mem(max_size, _new_size_of_arr, MemPool()->get_iterator_before(s_ref));
	if (new_ref != s_ref) {
		throw std::bad_alloc();
	}
}

void SimpleMathApp::Core::tree_ptr::realloc_plus(size_t _plus_new_size_of_arr)
{
	if (s_ref == nullptr) return alloc(_plus_new_size_of_arr);
	size_t num_of_bytes = 0;
	ref_iterator this_ref = MemPool()->get_iterator_before(s_ref);
	this_ref++;
	ref_iterator next_ref = this_ref;
	next_ref++;
	num_of_bytes = next_ref->shift() - s_ref->shift();
	if (check_if_can_continue(max_size, num_of_bytes / max_size + _plus_new_size_of_arr)) return;

	_ref *new_ref = MemPool()->relocate_mem(max_size, num_of_bytes / max_size + _plus_new_size_of_arr, MemPool()->get_iterator_before(s_ref));
	if (new_ref != s_ref) {
		throw std::bad_alloc();
	}
}

void SimpleMathApp::Core::tree_ptr::srink_to_last_ref()
{
	if (this->is_nullptr()) return;
	ref_iterator this_ref = MemPool()->get_iterator_before(s_ref);
	this_ref++;
	ref_iterator next_ref = this_ref;
	next_ref++;
	if (next_ref->ref_count()) {
		next_ref = MemPool()->get_ref_table().emplace_after(this_ref, _ref());
	}	
	next_ref->shift() = this->last_ref;
}

bool SimpleMathApp::Core::tree_ptr::is_nullptr()
{
	return s_ref == nullptr || !s_ref->ref_count();//в принципе счётчик ссылок не может быть равен нулю при наличии указателя.
}

bool SimpleMathApp::Core::tree_ptr::is_nullptr() const
{
	return s_ref == nullptr || !s_ref->ref_count();
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr SimpleMathApp::Core::tree_ptr::push_obj(math_obj *_obj, size_t _size)
{
	if (this->is_nullptr()) return __leaf_ptr();
	size_t obj_size = _obj->get_sizeof();	
	if (_size > obj_size) {
		obj_size = _size;
	}		
	ref_iterator this_ref = MemPool()->get_iterator_before(s_ref);
	this_ref++;
	ref_iterator next_ref = this_ref;
	next_ref++;
	if (next_ref->shift() && (last_ref + obj_size >= next_ref->shift())) {
		this->realloc_plus();
	}
	uint16_t virt_ref = _obj->place_copy_to(this->get_real_ptr(last_ref));
	__leaf_ptr out = __leaf_ptr(*this, last_ref + virt_ref);
	last_ref += obj_size;
	return out;
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::get_ptr_unsafe()
{
	return  ((math_obj*)((uint8_t*)(this->get_real_ptr()) + virtual_ref))->get_this();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::operator->()
{
	return  ((math_obj*)((uint8_t*)(this->get_real_ptr()) + virtual_ref))->get_this();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::operator->() const
{
	return  ((math_obj*)((uint8_t*)(this->get_real_ptr()) + virtual_ref))->get_this();
}

SimpleMathApp::Core::tree_ptr SimpleMathApp::Core::tree_ptr::operator=(const tree_ptr &_right)
{
	if (this == &_right)return *this;
	if (!this->is_nullptr()) //Если при присвоении указателя _right, этот указатель был инициализирован - уменьшить счётчик ссылок.
		if (s_ref->ref_count() > 1) {
			s_ref->ref_count()--;
		}
		else if (s_ref->ref_count() == 1) {
			math_obj* ptr = this->get_ptr_unsafe();
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "tree_ptr destructor from '=' for " << GetStrForTypeFlag(ptr->get_class_type()) << ". Shift is: " << s_ref->shift() << ". Real pointer is: " << get_real_ptr() << ". Virtual pointer is: " << ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
			}
			ptr->~math_obj();
		}
		this->s_ref = _right.s_ref;
		this->last_ref = _right.last_ref;
		this->virtual_ref = _right.virtual_ref;
		if (this->is_nullptr()) return *this;
		this->s_ref->ref_count()++;
		return *this;
}

bool SimpleMathApp::Core::tree_ptr::contains(const __leaf_ptr &_leaf)
{	
	return this->s_ref == _leaf.s_ref;
}

bool SimpleMathApp::Core::tree_ptr::is(const __leaf_ptr &_leaf)
{
	return this->contains(_leaf)&&!_leaf.relative_ref;
}
