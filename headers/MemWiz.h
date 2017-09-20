#pragma once
#ifndef MEMWIZ_H
#define MEMWIZ_H
//#define _TEST
#include "common.h"
#include <forward_list>

namespace MemoryControl {		
	//����� ������ (�������, �������) �� ����������������� ������� ������, ������ ������������ ��������� �� ����� ���.
	struct _ref {
	private:			

		uint32_t* temp_count;
		uint16_t size;
		//������, �������� ������������� ����� � �������� ������� ������. �� ��������� ���������������� ���� �������, � ������������ ����������.
		uint32_t* arr_count;		
		//��������� ������������� �������
		void init();
	public:
		//����� ���������� ��������� �� ������
		_ref* get_ref_ptr();
		//����������� �� ��������� �������������� ������ ����� ����������, ��������� ����� � �������� �������.
		_ref() throw(...);
		_ref(const _ref&)throw(...);
		_ref(const _ref&&);
		~_ref();
		//���������� �������� ������ �� ������
		uint32_t& shift();
		//���������� �������� ������� �������� �� ������. �� ��������� - �������� ��������� ��������
		uint32_t& ref_count(uint16_t _index=1);
		//����� ����������� ����� ��������� �� �������� ��������� _plus (=10 �� ���������) 
		void alloc_more_count(uint16_t _plus = 10) throw(...);
		//����� ���������� ������ �������� �� ������ ������ ��������, ���� �������� ��� - ���������� ������ ��� ��������������.
		uint16_t find_zero_count();
		_ref& operator= (const _ref&)throw(...);	
		uint16_t sizeof_counter_array();

	};
	//�������� ������������ ������ ������
	typedef  std::forward_list<MemoryControl::_ref>::iterator ref_iterator;

	class _memory_interface {
	private:
		template<class T>
		friend class smart_ptr_weak;	
		template<class T>
		friend class smart_ptr_strong;

		//������ ������(�������), ������ ������������ block
		std::forward_list<_ref> reference_table;
		//��������� �� ����� ���
		void *block;		
		//��������� �� ����� ������ ����
		void *block_end;
		//
		void allocate_new_block()throw(...);	
	public:
		//��������� �������� ������ ������ _size_of_type*_size_of_arr � ����� ���� � ���������� ��������� �� ������������� �����.
		_ref* allocate_mem(size_t _size_of_type, size_t _size_of_arr = 1);
		/*��������� ���������� ��������� ����������� ������� ������, �� ������� ��������� ��������� ������ ����� _ref_before, �� ����� ����� � ������������ � ����������� _size_of_type*_new_size_of_arr.
		��� ���� ������� ��������� �� ��������� (�� ������:). ����� ���������� ��������� �� ������������� ����� ��� ������ �������� ����������.*/
		_ref* relocate_mem(size_t _size_of_type, size_t _new_size_of_arr, ref_iterator _ref_before);
		//����� ��������� �������� ������ �������, ����������� �� ����� ���������� ���������.
		ref_iterator get_iterator_before(_ref*);

		//����������� �� ��������� ����� ����� ��� ������ � �������������� ������ �������
		_memory_interface()throw(...);
		//���������� ����������� ��� ������, ��� ����������� �� ���� ���������� �� ��� �������� ������ ��� ���.
		~_memory_interface();

		//����� ������� � �������
		std::forward_list<_ref>& get_ref_table();
		//����� ������� � ��������� �� ����� ���
		void* get_block();
		//����� ������� � ��������� �� ����� ����.
		void* get_block_end();
	};
	//������ � ����������� ���������� ������ ���� ������
	 inline _memory_interface* MemPool() {
		static _memory_interface* mem_wiz = new _memory_interface();
		return mem_wiz;
	}
	//������������� ������ ���� ������. �������������.		
	 _memory_interface& InitializeMemPool();
	 //��������������� ������ ���� ������. ��� ���������� ����������� ��� ����� ���������.
	 _memory_interface& ReinitializeMemPool();

	 void DestMemPool();

	 size_t ReadInitialfromFile();
	 
	 inline size_t& init_memory() {
		 static size_t init_memory = ReadInitialfromFile(); //� ��
		 return init_memory;
	 }
}
#endif //MEMWIZ_H