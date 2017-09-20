#pragma once
#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "common.h"
#include "smart_ptr.h"


namespace SimpleMathApp {
	using namespace MemoryControl;
	class data_list
	{
	public:
		class iterator;
	private:
		class _data_bit
		{			
		private:
			friend class data_list;
			friend class data_list::iterator;


			smart_ptr_weak<_data_bit> left;
			smart_ptr_weak<_data_bit> right;
			std::wstring in;
			std::wstring out;			
			//Core::tree_ptr object;			
			
		public:
			_data_bit();
			_data_bit(std::wstring&);
			_data_bit(smart_ptr_weak<_data_bit>&);
			_data_bit(smart_ptr_weak<_data_bit>&, smart_ptr_weak<_data_bit>&);
			_data_bit(smart_ptr_weak<_data_bit>&, smart_ptr_weak<_data_bit>&, std::wstring&);
			_data_bit(const _data_bit&);
			~_data_bit();

			/*Метод сравнения строки in (ввод) с заданной строкой. Аналогичен методу compare класса wstring
			Метод возвращает:
			- 0 - строки совпадают
			- разницу между первыми различными символами*/
			int compare_in(std::wstring&);

			/*Метод сравнения строки out (вывод) с заданной строкой. Аналогичен методу compare класса wstring
			Метод возвращает:
			- 0 - строки совпадают
			- разницу между первыми различными символами*/
			int compare_out(std::wstring&);

			void set_in(std::wstring&);
			std::wstring get_in();

			void set_out(std::wstring&);
			std::wstring get_out();

			int build_handler();

		};

		friend bool operator== (const iterator& _left, const iterator& _right);
		friend bool operator!= (const iterator& _left, const iterator& _right);

		size_t length;
		smart_ptr_weak<_data_bit> begin_bit;
		smart_ptr_weak<_data_bit> top_bit;
		smart_ptr_weak<_data_bit> end_bit;		
			

	public:

		class iterator
		{
		private:
			friend class data_list;
			smart_ptr_weak<_data_bit> letter;
			data_list* this_data_list;
			iterator(smart_ptr_weak<_data_bit>&, data_list*);
			
		public:
			iterator();			
			iterator(const iterator&);
			iterator(const iterator&&);
			~iterator();
			
			void set_in(std::wstring&);
			std::wstring get_in();

			void set_out(std::wstring&);
			std::wstring get_out();

			/*Core::tree_ptr get_obj();
			void assing_obj(Core::math_obj * _obj);*/

			int compare_in(std::wstring&original);
			int compare_out(std::wstring&original);

			iterator emplace_after(std::wstring&);
			int build();

			iterator &operator= (const iterator& _right);			
			//постфиксный
			iterator &operator++ (int);
			//префиксный
			iterator &operator++ ();
			//постфиксный
			iterator &operator-- (int);
			//префиксный
			iterator &operator-- ();
			_data_bit* operator->();

			friend bool operator== (const iterator& _left, const iterator& _right);
			friend bool operator!= (const iterator& _left, const iterator& _right);

			operator smart_ptr_weak<_data_bit>() { return this->letter; }//почему-то не получается вынести реализацию за пределы класса. студия не видит ей же созданные определения

		};		

		data_list();
		~data_list();

		size_t size();
		void push_back(std::wstring&);
		void push_left(/*Core::tree_ptr&*/);
		

		iterator before_begin();
		iterator top();
		iterator end();

	};
	typedef data_list::iterator data_iterator;
	bool operator== (const SimpleMathApp::data_list::iterator& _left, const SimpleMathApp::data_list::iterator& _right);
	bool operator!= (const SimpleMathApp::data_list::iterator& _left, const SimpleMathApp::data_list::iterator& _right);



	inline data_list* MathData() {
		static data_list* data= new data_list(); 
		return data;
	}

	void InitializeMathData();
	void DeleteMathData();
	void ReinitializeMathData();

}



#endif // !DATA_LIST_H



