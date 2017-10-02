#include "..\headers\math_obj.h"
using namespace MemoryControl;


SimpleMathApp::Core::tree_ptr::__leaf_ptr::__leaf_ptr() :s_ref(nullptr), relative_ref(), count_index()
{
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr::__leaf_ptr(const tree_ptr & _tree) : s_ref(_tree.s_ref), relative_ref(_tree.virtual_ref), count_index(1)
{
	if (!this->is_nullptr())
		s_ref->ref_count(count_index)++;
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr::__leaf_ptr(tree_ptr &_tree, math_obj *_obj)
{
	leaf_ptr new_leaf = _tree.push_obj(_obj);
	s_ref = new_leaf.s_ref;
	relative_ref = new_leaf.relative_ref;
	count_index = new_leaf.count_index;
	if (!this->is_nullptr())
		s_ref->ref_count(count_index)++;
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr::__leaf_ptr(const tree_ptr &_tree, uint32_t _shift) :s_ref(_tree.s_ref), relative_ref(_shift)
{
	if (!this->is_nullptr()) {
		count_index = s_ref->find_zero_count();
		s_ref->ref_count(count_index)++;
	}
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr::__leaf_ptr(const __leaf_ptr &_leaf) :s_ref(_leaf.s_ref), relative_ref(_leaf.relative_ref), count_index(_leaf.count_index)
{
	if (!this->is_nullptr()) {
		s_ref->ref_count(count_index)++;
	}
}


void * SimpleMathApp::Core::tree_ptr::__leaf_ptr::get_real_ptr()
{
	return (this->is_nullptr()) ? nullptr : ((int8_t*)MemPool()->get_block() + s_ref->shift() + relative_ref);
}

void * SimpleMathApp::Core::tree_ptr::__leaf_ptr::get_real_ptr() const
{
	return (this->is_nullptr()) ? nullptr : ((int8_t*)MemPool()->get_block() + s_ref->shift() + relative_ref);
}



SimpleMathApp::Core::tree_ptr::__leaf_ptr::~__leaf_ptr()
{
	if (!this->is_nullptr()) {
		if (s_ref->ref_count(count_index) > 1) {
			s_ref->ref_count(count_index)--;
		}
		else if (s_ref->ref_count(count_index) == 1) {
			math_obj* ptr = this->get_ptr_unsafe();
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "leaf_ptr destructor for " << GetStrForTypeFlag(ptr->get_class_type()) << ". Shift is: " << s_ref->shift() << ". Relative_ref is: " << relative_ref << ". Real pointer is: " << get_real_ptr() << ". Virtual pointer is: " << ptr << ". Reference count: " << s_ref->ref_count(count_index) << std::endl;
			}
			s_ref->ref_count(count_index)--;
			ptr->~math_obj();
		}
	}
}

bool SimpleMathApp::Core::tree_ptr::__leaf_ptr::is_nullptr()
{
	return s_ref == nullptr;
}

bool SimpleMathApp::Core::tree_ptr::__leaf_ptr::is_nullptr() const
{
	return s_ref == nullptr;
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::__leaf_ptr::get_ptr_unsafe()
{
	return (math_obj*)this->get_real_ptr();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::__leaf_ptr::get_ptr_unsafe() const
{
	return (math_obj*)this->get_real_ptr();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::__leaf_ptr::operator->()
{
	return (math_obj*)this->get_real_ptr();
}

SimpleMathApp::Core::math_obj * SimpleMathApp::Core::tree_ptr::__leaf_ptr::operator->() const
{
	return (math_obj*)this->get_real_ptr();
}

SimpleMathApp::Core::tree_ptr::__leaf_ptr & SimpleMathApp::Core::tree_ptr::__leaf_ptr::operator=(const __leaf_ptr &_leaf)
{
	if (this == &_leaf)return *this;
	if (!this->is_nullptr()) {
		if (s_ref->ref_count(count_index) > 1)
			this->s_ref->ref_count(count_index)--;
		else if (s_ref->ref_count(count_index) == 1) {
			math_obj* ptr = this->get_ptr_unsafe();
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "leaf_ptr destructor from '=' for " << GetStrForTypeFlag(ptr->get_class_type()) << ". Shift is: " << s_ref->shift() << ". Relative_ref is: " << relative_ref << ". Real pointer is: " << get_real_ptr() << ". Virtual pointer is: " << ptr << ". Reference count: " << s_ref->ref_count(count_index) << std::endl;
			}
			s_ref->ref_count(count_index)--;
			ptr->~math_obj();
		}
	}	
	s_ref = _leaf.s_ref;
	relative_ref = _leaf.relative_ref;
	count_index = _leaf.count_index;
	if (this->is_nullptr()) return *this;
	s_ref->ref_count(count_index)++;
	return *this;
}

bool SimpleMathApp::Core::tree_ptr::__leaf_ptr::operator==(const __leaf_ptr &_leaf) const
{
	return (this->s_ref== _leaf.s_ref&&this->relative_ref == _leaf.relative_ref);
}
