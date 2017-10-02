#pragma once
#include "math_obj.h"
#ifndef OPERATIONS_H
#define OPERATIONS_H
namespace SimpleMathApp {
	namespace Core {

		class operations :
			public math_obj
		{
		private:
			leaf_ptr prioritize_rec(int& _p);
		protected:
			leaf_ptr point_left;	//левый операнд
			leaf_ptr point_right;	//правый операнд
			leaf_ptr point_collar; //указатель на предыдущую операцию

			
		public:
			operations();
			operations(const leaf_ptr& _pl);
			operations(const leaf_ptr& _pl, const leaf_ptr& _pr);
			operations(const leaf_ptr& _pl, const leaf_ptr& _pr, const leaf_ptr& _pc);

			virtual ~operations();
			leaf_ptr get_pl();
			void set_pl(const leaf_ptr& _pointer);
			leaf_ptr get_pr();			
			void set_pr(const leaf_ptr& _pointer);
			leaf_ptr get_pc();		
			void set_pc(const leaf_ptr& _pointer);

			//virtual leaf_ptr get_result() = 0;

			bool define_operation(const leaf_ptr& _high,const leaf_ptr& _low,const leaf_ptr& _obj,const leaf_ptr& _this);
			leaf_ptr prioritize(int _priority);
			virtual int get_priority() = 0;

			virtual math_obj* copy(math_obj* _original);

			virtual std::wstring get_formal_entry();
		};

		class addition :
			public operations
		{
		public:
			addition();
			addition(const leaf_ptr& _pl);
			addition(const leaf_ptr& _pl, const leaf_ptr& _pr);
			addition(const leaf_ptr& _high, const leaf_ptr& _low, const leaf_ptr& _obj, const leaf_ptr& _this) throw (...);
			addition(const addition&);
			~addition();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();			
			virtual std::wstring get_name();
			
			virtual int get_priority();
			virtual uint16_t place_copy_to(void * _ptr);			

			virtual math_obj* process(tree_ptr&);
			
		};

		class subtraction :
			public operations
		{
		public:
			subtraction();
			subtraction(const leaf_ptr& _pl);
			subtraction(const leaf_ptr& _pl, const leaf_ptr& _pr);
			subtraction(const leaf_ptr& _pl, const leaf_ptr& _pr, const leaf_ptr& _pc);
			subtraction(const leaf_ptr& _high, const leaf_ptr& _low, const leaf_ptr& _obj, const leaf_ptr& _this) throw (...);
			subtraction(const subtraction&);
			~subtraction();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();
			virtual std::wstring get_name();
			
			virtual int get_priority();
			virtual uint16_t place_copy_to(void * _ptr);		

			virtual math_obj* process(tree_ptr&);
			
		};

		class multiplication :
			public operations
		{
		public:
			multiplication();
			multiplication(const leaf_ptr& _pl);
			multiplication(const leaf_ptr& _pl, const leaf_ptr& _pr);
			multiplication(const leaf_ptr& _high, const leaf_ptr& _low, const leaf_ptr& _obj, const leaf_ptr& _this) throw (...);
			multiplication(const multiplication&);
			~multiplication();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();
			virtual std::wstring get_name();
			
			virtual int get_priority();
			virtual uint16_t place_copy_to(void * _ptr);	

			virtual math_obj* process(tree_ptr&);
		};

		class division :
			public operations
		{
		public:
			division();
			division(const leaf_ptr& _pl);
			division(const leaf_ptr& _pl, const leaf_ptr& _pr);
			division(const leaf_ptr& _high, const leaf_ptr& _low, const leaf_ptr& _obj, const leaf_ptr& _this) throw (...);
			division(const division&);
			~division();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();
			virtual std::wstring get_name();
			
			virtual int get_priority();
			virtual uint16_t place_copy_to(void * _ptr);	

			virtual math_obj* process(tree_ptr&);
		};

		class power :
			public operations
		{
		public:
			power();
			power(const leaf_ptr& _pl);
			power(const leaf_ptr& _pl, const leaf_ptr& _pr);
			power(const leaf_ptr& _high, const leaf_ptr& _low, const leaf_ptr& _obj, const leaf_ptr& _this) throw (...);
			power(const power&);
			~power();

			virtual flags get_class_type();
			virtual size_t get_sizeof();
			virtual math_obj* get_this();
			virtual std::wstring get_name();
			
			virtual int get_priority();
			virtual uint16_t place_copy_to(void * _ptr);			

			virtual math_obj* process(tree_ptr&);
		};
	}
}

#endif // !OPERATIONS_H









