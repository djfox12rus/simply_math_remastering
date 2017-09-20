#pragma once
#ifndef SMART_PTR_H
#define SMART_PTR_H
#include "MemWiz.h"
#include <typeinfo>

namespace MemoryControl {

	template<class T>
	class smart_ptr_weak
	{
	protected:
		_ref* s_ref;		

	public:
		//Конструктор по умолчанию. 
		smart_ptr_weak() :s_ref(nullptr)
		{
			/*if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Default smart_ptr_weak constructor for " << r1.name()<<". " << std::endl;
			}	*/		
		}
		/*Конструктор, принимающий в качестве параметра объект для размещения в памяти. !Обязательно требуется конструктор копирования для Т класса (с const ссылкой!)! 
		*/
		smart_ptr_weak(T& _obj) : s_ref(nullptr)
		{
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Smart_ptr_weak constructor with arg for " << r1.name() << ". ";
			}
			s_ref = MemPool()->allocate_mem(sizeof(T));
			void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
			T* virt_ptr = new (temp_ptr) T(_obj);			
			//*virt_ptr = _obj;
		}
		//Конструктор копирования. Увеличивает счётчик ссылок.
		smart_ptr_weak(const smart_ptr_weak &_original) : s_ref(_original.s_ref)
		{
			if (!this->is_nullptr()) {
				this->s_ref->ref_count()++;				
			}
		}
		//Конструктор перемещения. НЕ увеличивает счётчик ссылок, переданный указатель более недействителен (нулевой).
		smart_ptr_weak(const smart_ptr_weak &&_movable) : s_ref(_movable.s_ref)
		{					
			if (!this->is_nullptr()) {
				this->s_ref->ref_count()++;				
			}
		}
		//Деструктор. Уменьшает счётчик ссылок. По достижении счётчиком нуля - вызывает деструктор ~T().
		~smart_ptr_weak()
		{
			if (!this->is_nullptr()) {
				if (s_ref->ref_count() > 1) {
					s_ref->ref_count()--;	
					/*if (SimpleMathApp::TestLog::TEST()) {
						const std::type_info& r1 = typeid(T);
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Smart_ptr_weak destructor for " << r1.name() << ". Shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
					}*/
				}
				else if (s_ref->ref_count() == 1) {
					T* ptr = (T*)((int8_t*)MemPool()->block + (this->s_ref->shift()));
					s_ref->ref_count()--;					
					ptr->~T();					
					if (SimpleMathApp::TestLog::TEST()) {
						const std::type_info& r1 = typeid(T);
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Smart_ptr_weak destructor for " << r1.name() << ". Shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
					}
				}
			}
			else {
				if (SimpleMathApp::TestLog::TEST()) {
					const std::type_info& r1 = typeid(T);					
					SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "NULL/EXCESS Smart_ptr_weak destructor for " << r1.name() << "." << std::endl;
				}
			}
		}

		//Метод возвращает true, если smart_ptr_weak ни на что не указывает.
		bool is_nullptr()
		{
			return s_ref == nullptr;
		}
		//Небезопасно! Нельзя удалять ("забывать") сам smart_pointer! Нельзя использовать delete!
		T* get_ptr_unsafe()
		{			
			return (s_ref == nullptr) ? nullptr : ((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Перенаправление вида (Т*)-> 
		T* operator->()
		{			
			return (s_ref == nullptr) ? nullptr : ((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Перенаправление вида *(Т*)
		T& operator*()
		{			
			T zero = T();//тут что-то надо придумать. решение со статик не подходит для статических объектов, ссылающихся на пул памяти. Он очищается раньше статиков.
			return (this->is_nullptr()) ? zero : *((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Оператор присваивания. Уменьшает счётчик ссылок для левостороннего объекта (вызывается деструктор при достижении нуля) и увеличивает счётчик для правостороннего объекта.
		smart_ptr_weak<T>& operator=(const smart_ptr_weak<T>& _right)
		{
			/*if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Call smart_ptr_weak::operator= for " << r1.name() << ". ";
			}*/
			if (this == &_right)
				return *this; //Проверка равентсва по адресу.
			if (!this->is_nullptr()) {
				if (s_ref->ref_count() > 1) {
					s_ref->ref_count()--;
					/*if (SimpleMathApp::TestLog::TEST()) {
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Old shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count();
					}*/
				}
				else if (s_ref->ref_count() == 1) {
					T* ptr = (T*)((int8_t*)MemPool()->block + (this->s_ref->shift()));
					s_ref->ref_count()--;
					ptr->~T();					
					/*if (SimpleMathApp::TestLog::TEST()) {
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Old shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". (Dest)Reference count: " << s_ref->ref_count();
					}*/
				}
			}
			this->s_ref = _right.s_ref;
			if (this->is_nullptr()) return *this;
			this->s_ref->ref_count()++;
			/*if (SimpleMathApp::TestLog::TEST()) {
				void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "New shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
			}	*/		
			return *this;
		}
		
		bool compare(const smart_ptr_weak<T>* _right) {		
			return (this == _right||this->s_ref == _right->s_ref);
		}

		/*Метод, принимающий в качестве параметра объект для размещения в памяти. !Обязательно требуется конструктор копирования для Т класса (с const ссылкой!)!
		Срабатывает только для нулевого указателя.
		Если копирование заблокировано - разместить объект с помощью smart_ptr_strong(T _obj) и воспользоваться конструктором smart_ptr_weak(smart_ptr_strong) */
		void place(T& _obj) {
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Call smart_ptr_weak::place for " << r1.name() << ". ";
			}
			if (!this->is_nullptr()) return;
			s_ref = MemPool()->allocate_mem(sizeof(T));
			void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
			T* virt_ptr = new (temp_ptr) T(_obj);			
		}
	};

	





	template<class T>
	class smart_ptr_strong {
	protected:
		_ref* s_ref;
	public:
		//Конструктор по умолчанию. Конструирует объект Т() на месте. !Обязательно требуется открытый коструктор по умолчанию для Т()!
		smart_ptr_strong() :s_ref(nullptr)
		{
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Default smart_ptr_strong constructor for " << r1.name() << ". ";
			}
			s_ref = MemPool()->allocate_mem(sizeof(T));
			void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
			T* virt_ptr = new (temp_ptr) T();			
		}
		/*Конструктор, принимающий в качестве параметра объект для размещения в памяти. !Обязательно требуется конструктор копирования для Т класса (с const ссылкой!)!
		Если копирование заблокировано - пользоваться конструктором по умолчанию и размещать нужный объект через operator*(smart_ptr_strong) и оператор присваивания для T класса.*/
		smart_ptr_strong(T& _obj) : s_ref(nullptr)
		{
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "smart_ptr_strong constructor with arg for " << r1.name() << ". ";
			}
			s_ref = MemPool()->allocate_mem(sizeof(T));
			void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
			T* virt_ptr = new (temp_ptr) T(_obj);			
			//*virt_ptr = _obj;
		}
		//Конструктор копирования. Увеличивает счётчик ссылок.
		smart_ptr_strong(const smart_ptr_strong &_original) : s_ref(_original.s_ref)
		{
			if (!this->is_nullptr()) {
				this->s_ref->ref_count()++;				
			}
		}
		//Конструктор перемещения. НЕ увеличивает счётчик ссылок, переданный указатель более недействителен (нулевой).
		smart_ptr_strong(const smart_ptr_strong &&_movable) : s_ref(_movable.s_ref)
		{			
			if (!this->is_nullptr()) {
				this->s_ref->ref_count()++;				
			}
		}
		//Деструктор. Уменьшает счётчик ссылок. По достижении счётчиком нуля - вызывает деструктор ~T().
		~smart_ptr_strong()
		{
			if (!this->is_nullptr()) {
				if (s_ref->ref_count() > 1) {
					s_ref->ref_count()--;					
				}
				else if (s_ref->ref_count() == 1) {
					T* ptr = (T*)((int8_t*)MemPool()->block + (this->s_ref->shift()));
					s_ref->ref_count()--;
					ptr->~T();					
					if (SimpleMathApp::TestLog::TEST()) {
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "smart_ptr_strong destructor for " << r1.name() << ". Shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
					}
				}
			}
		}

		//Метод возвращает true, если smart_ptr_weak ни на что не указывает.
		bool is_nullptr()
		{
			return s_ref == nullptr;
		}
		//Небезопасно! Нельзя удалять ("забывать") сам smart_pointer! Нельзя использовать delete!
		T* get_ptr_unsafe()
		{			
			return (s_ref == nullptr) ? nullptr : ((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Перенаправление вида (Т*)-> 
		T* operator->()
		{			
			return (s_ref == nullptr) ? nullptr : ((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Перенаправление вида *(Т*)
		T& operator*()
		{			
			T zero = T();//тут что-то надо придумать. решение со статик не подходит для статических объектов, ссылающихся на пул памяти. Он очищается раньше статиков.
			return (this->is_nullptr()) ? zero : *((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
		}
		//Оператор присваивания. Уменьшает счётчик ссылок для левостороннего объекта (вызывается деструктор при достижении нуля) и увеличивает счётчик для правостороннего объекта.
		smart_ptr_strong<T>& operator=(const smart_ptr_strong<T>& _right)
		{
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Call smart_ptr_strong::operator= for " << r1.name() << ". ";
			}
			if (this == &_right)
				return *this; //Проверка равентсва по адресу.
			if (!this->is_nullptr()) {
				if (s_ref->ref_count() > 1) {
					s_ref->ref_count()--;
					if (SimpleMathApp::TestLog::TEST()) {
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Old shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
					}
				}
				else if (s_ref->ref_count() == 1) {
					T* ptr = (T*)((int8_t*)MemPool()->block + (this->s_ref->shift()));
					s_ref->ref_count()--;
					ptr->~T();					
					if (SimpleMathApp::TestLog::TEST()) {
						void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
						SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Old shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". (Dest)Reference count: " << s_ref->ref_count() << std::endl;
					}
				}
			}
			this->s_ref = _right.s_ref;
			if (this->is_nullptr()) return *this;
			this->s_ref->ref_count()++;
			if (SimpleMathApp::TestLog::TEST()) {
				void *temp_ptr = (void*)((int8_t*)MemPool()->block + (s_ref->shift()));
				SimpleMathApp::TestLog::LOG_STREAM() << "New shift is: " << s_ref->shift() << ". Real pointer is: " << temp_ptr << ". Reference count: " << s_ref->ref_count() << std::endl;
			}
			return *this;
		}

		

		operator smart_ptr_weak<T>() {
			if (SimpleMathApp::TestLog::TEST()) {
				const std::type_info& r1 = typeid(T);
				SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Call smart_ptr_strong::operator smart_ptr_weak() for " << r1.name() << ". ";
			}
			if (this->is_nullptr()) return smart_ptr_weak<T>();
			T obj = *((T*)((int8_t*)MemPool()->block + (this->s_ref->shift())));
			return smart_ptr_weak<T>(obj);
		}
	};

	

}
#endif // !SMART_PTR_H