#pragma once
#include "math_obj.h"
#ifndef CONSTANT_EXPRESSIONS_H
#define CONSTANT_EXPRESSIONS_H
namespace SimpleMathApp {
	namespace Core {

		class expression :
			public math_obj
		{
		protected:
			leaf_ptr point_left;
		public:
			expression();
			expression(const leaf_ptr& _pl);
			expression(const expression&);
			virtual ~expression();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();			
			virtual leaf_ptr get_pl();
			virtual void set_pl(const leaf_ptr& _pointer);
			virtual uint16_t place_copy_to(void * _ptr);
			virtual math_obj* copy(math_obj* _original);

			virtual std::wstring get_formal_entry();

			virtual math_obj* process(tree_ptr&);
			
		};

		class real :
			public math_obj
		{
		protected:			
			long double num;
		public:
			real();
			real(const long double& _num);
			real(const real&);
			virtual ~real();
						
			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();			
			virtual long double get_num();			
			virtual void set_num(long double& _num);
			virtual uint16_t place_copy_to(void * _ptr);
			virtual math_obj* copy(math_obj* _original);

			virtual std::wstring get_formal_entry();

			virtual math_obj* process(tree_ptr&);
		};

		class constant_real :
			public expression
		{
		protected:
			std::wstring name;
			long double num;
			//leaf_ptr point_left;
		public:
			constant_real();			
			constant_real(const leaf_ptr& _pl, const long double& _num, const std::wstring& _name);
			constant_real(const constant_real&);
			virtual ~constant_real();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();			
			virtual std::wstring get_name();			
			virtual void set_name(const std::wstring& _name);
			virtual long double get_num();			
			virtual void set_num(long double& _num);
			virtual uint16_t place_copy_to(void * _ptr);
			virtual math_obj* copy(math_obj* _original);

			virtual std::wstring get_formal_entry();

			virtual math_obj* process(tree_ptr&);
		};

	}
}

#endif // !CONSTANT_EXPRESSIONS_H





