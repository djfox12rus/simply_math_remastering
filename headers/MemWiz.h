#pragma once
#ifndef MEMWIZ_H
#define MEMWIZ_H
//#define _TEST
#include "common.h"
#include <forward_list>

namespace MemoryControl {		
	//Класс ссылок (сдвигов, адресов) на зарезервированный участок памяти, взятых относительно указателя на общий пул.
	struct _ref {
	private:			

		uint32_t* temp_count;
		uint16_t size;
		//Массив, хранящий относительный сдвиг и счётчики внешних ссылок. По умолчанию инициализируется один счетчик, с возможностью расширения.
		uint32_t* arr_count;		
		//Начальная инициализация массива
		void init();
	public:
		//Метод возвращает указатель на объект
		_ref* get_ref_ptr();
		//Конструктор по умолчанию инициализирует массив двумя элементами, хранящими сдвиг и основной счётчик.
		_ref() throw(...);
		_ref(const _ref&)throw(...);
		_ref(const _ref&&);
		~_ref();
		//Возвращает значения сдвига по ссылке
		uint32_t& shift();
		//Возвращает значения нужного счётчика по ссылке. По умолчанию - значение основного счётчика
		uint32_t& ref_count(uint16_t _index=1);
		//Метод увеличивает число счётчиков на величину параметра _plus (=10 по умолчанию) 
		void alloc_more_count(uint16_t _plus = 10) throw(...);
		//Метод возвращает индекс нулевого на данный момент счётчика, если такового нет - выделяется память под дополнительные.
		uint16_t find_zero_count();
		_ref& operator= (const _ref&)throw(...);	
		uint16_t sizeof_counter_array();

	};
	//Итератор односвязного списка ссылок
	typedef  std::forward_list<MemoryControl::_ref>::iterator ref_iterator;

	class _memory_interface {
	private:
		template<class T>
		friend class smart_ptr_weak;	
		template<class T>
		friend class smart_ptr_strong;

		//Список ссылок(адресов), взятых относительно block
		std::forward_list<_ref> reference_table;
		//Указатель на общий пул
		void *block;		
		//указатель на конец общего пула
		void *block_end;
		//
		void allocate_new_block()throw(...);	
	public:
		//Интерфейс выделяет память объёма _size_of_type*_size_of_arr в общем пуле и возвращает указатель на относительный адрес.
		_ref* allocate_mem(size_t _size_of_type, size_t _size_of_arr = 1);
		/*Интерфейс производит релокацию выделенного участка памяти, на которую указывает следующая запись после _ref_before, на новое место в соответствии с требованием _size_of_type*_new_size_of_arr.
		При этом внешние указатели не сбиваются (не должны:). Метод возвращает указатель на относительный адрес для точной проверки последнего.*/
		_ref* relocate_mem(size_t _size_of_type, size_t _new_size_of_arr, ref_iterator _ref_before);
		//Метод вычисляет итератор списка адресов, указывающий на адрес предыдущий заданному.
		ref_iterator get_iterator_before(_ref*);

		//Конструктор по умолчанию задаёт общий пул памяти и инициализирует список адресов
		_memory_interface()throw(...);
		//Деструктор освобождает всю память, вне зависимости от того обнулились ли все счётчики ссылок или нет.
		~_memory_interface();

		//Метод доступа к адресам
		std::forward_list<_ref>& get_ref_table();
		//Метод доступа к указателю на общий пул
		void* get_block();
		//Метод доступа к указателю на конец пула.
		void* get_block_end();
	};
	//Доступ к статической переменной общего пула памяти
	 inline _memory_interface* MemPool() {
		static _memory_interface* mem_wiz = new _memory_interface();
		return mem_wiz;
	}
	//Инициализация общего пула памяти. Необязательна.		
	 _memory_interface& InitializeMemPool();
	 //Реинициализация общего пула памяти. Вся информация хранившаяся там ранее пропадает.
	 _memory_interface& ReinitializeMemPool();

	 void DestMemPool();

	 size_t ReadInitialfromFile();
	 
	 inline size_t& init_memory() {
		 static size_t init_memory = ReadInitialfromFile(); //в Кб
		 return init_memory;
	 }
}
#endif //MEMWIZ_H