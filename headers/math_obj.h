#pragma once
#ifndef MATH_OBJ_H
#define MATH_OBJ_H

#include "smart_ptr.h"

namespace SimpleMathApp {
	namespace Core {		

		static const uint32_t max_size = 100;

		class math_obj;
		class tree_ptr
		{
		private:
			MemoryControl::_ref* s_ref;
			uint32_t last_ref;
			uint16_t virtual_ref;

			void* get_real_ptr(uint32_t _plus_ref=0);
			void* get_real_ptr(uint32_t _plus_ref = 0)const;
			bool check_if_can_continue(size_t, size_t);
		public:
			class __leaf_ptr
			{
			private:
				friend class tree_ptr;
				MemoryControl::_ref* s_ref;
				uint32_t relative_ref;
				uint16_t count_index;

				void* get_real_ptr();
				void* get_real_ptr() const;

			public:
				__leaf_ptr();
				__leaf_ptr(const tree_ptr&);
				__leaf_ptr(tree_ptr&, math_obj*);
				__leaf_ptr(const tree_ptr&, uint32_t);
				__leaf_ptr(const __leaf_ptr&);
				~__leaf_ptr();

				bool is_nullptr();
				bool is_nullptr() const;
				math_obj* get_ptr_unsafe();
				math_obj* get_ptr_unsafe()const;

				math_obj* operator->();
				math_obj* operator->() const;
				__leaf_ptr& operator =(const __leaf_ptr&);

				bool operator == (const __leaf_ptr&)const;
			};

			tree_ptr();
			tree_ptr(math_obj*, size_t _size = 1);
			tree_ptr(const tree_ptr&);
			~tree_ptr();

			void alloc(size_t);
			void realloc(size_t);
			void realloc_plus(size_t _plus_new_size_of_arr = 1);
			void srink_to_last_ref();

			bool is_nullptr();
			bool is_nullptr() const;
			__leaf_ptr push_obj(math_obj*, size_t _size=0);

			math_obj* get_ptr_unsafe();

			math_obj* operator->();
			math_obj* operator->() const;
			tree_ptr operator =(const tree_ptr&);

			bool contains(const __leaf_ptr&);
			bool is(const __leaf_ptr&);

		};
		typedef tree_ptr::__leaf_ptr leaf_ptr;


		class math_obj
		{
		public:
			math_obj() {}
			virtual ~math_obj() {}

			//Метод get. ТИП
			virtual flags get_class_type() {
				return flags::base;
			}

			//Метод get. РАЗМЕР
			virtual size_t get_sizeof() = 0;

			//Метод get. ЭЛЕМЕНТ
			void* get_this_void() { return this; }

			virtual math_obj* get_this() { return this; }

			//Метод get. ИМЯ
			virtual std::wstring get_name() { return  std::wstring(); }
			//Метод assing. ИМЯ
			virtual void set_name(const std::wstring& _name) {}

			//Метод get. ЧИСЛО
			virtual long double get_num() { return 0; }
			//Метод assing. ЧИСЛО
			virtual void set_num(long double& _num) {}

			//Метод get. УКАЗАТЕЛЬ "левый рукав"
			virtual leaf_ptr get_pl() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "левый рукав"
			virtual void set_pl(const leaf_ptr & _pointer) {}


			//Метод get. УКАЗАТЕЛЬ "правый рукав"
			virtual leaf_ptr get_pr() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "правый рукав"
			virtual void set_pr(const leaf_ptr & _pointer) {}


			//Метод get. УКАЗАТЕЛЬ "воротник"
			virtual leaf_ptr get_pc() { return leaf_ptr(); }
			//Метод assing. УКАЗАТЕЛЬ "воротник"
			virtual void set_pc(const leaf_ptr & _pointer) {}

			virtual std::wstring get_formal_entry() = 0;

			virtual int get_priority() { return 0; }

			virtual uint16_t place_copy_to(void * _ptr) = 0;

			virtual math_obj* copy(math_obj* _original) = 0;
			
			virtual math_obj* process(tree_ptr&) = 0;

			

		};


	}
}




#endif // !MATH_OBJ_H



