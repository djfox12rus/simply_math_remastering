

#include "../headers/MemWiz.h"

using namespace MemoryControl;


namespace MemoryControl {
	void _memory_interface::allocate_new_block()
	{
		size_t old_size = ((uint8_t*)this->block_end - (uint8_t*)this->block);
		size_t new_size = old_size + init_memory() * 1024;
		void* new_block;
		uint32_t try_count = 0;
		do {
#ifdef _WINDOWS_
			new_block = VirtualAllocEx(GetCurrentProcess(), NULL, new_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else//!_WINDOWS_	
			new_block = calloc(new_size/1024, 1024);
#endif //_WINDOWS_			
			try_count++;
		} while (!new_block&&try_count < 5);
		if (!new_block) {
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << "Failed to allocate memory pool." << std::endl;
			}
			throw std::bad_alloc();
		}
		std::memcpy(new_block, this->block, old_size);

		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Memory pool deallocated at " << this->block <<". Old size: "<< old_size << std::endl;
		}

#ifdef _WINDOWS_
		VirtualFreeEx(GetCurrentProcess(), this->block, 0, MEM_RELEASE);
#else//!_WINDOWS_	
		free(block);
#endif//_WINDOWS_	
		this->block = new_block;
		block_end = (void*)((uint8_t*)this->block + new_size);

		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Memory pool reallocated at " << this->block << " from " << try_count << " try. Block ends at " << this->block_end << ". New size: " << new_size << std::endl;
		}
	}

	MemoryControl::_ref* MemoryControl::_memory_interface::allocate_mem(size_t _size_of_type, size_t _size_of_arr)
	{

		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Call for _memory_interface::allocate_mem(). ";
		}
		ref_iterator temp_ref = this->reference_table.before_begin();
		ref_iterator next_ref;
		//void *end = (int8_t*)mem_wiz.block + init_memory * 1024 - 8;
		size_t size = _size_of_type*_size_of_arr;
		while (temp_ref != this->reference_table.end()) {
			if (this->reference_table.empty()) {
				temp_ref = this->reference_table.emplace_after(temp_ref, _ref());
				next_ref = this->reference_table.emplace_after(temp_ref, _ref());
			}
			else {
				temp_ref++;
				next_ref = temp_ref;
				next_ref++;
			}
			if (!temp_ref->ref_count()) {//счётчик ссылок равен нулю				
				if (temp_ref->shift() || (temp_ref == this->reference_table.begin() && next_ref->shift())) {//если ранее память была использована/инициализирован указатель на конец предыдущего блока					
					if (next_ref == this->reference_table.end()) {//адреса кончились											
						uint32_t try_count = 0;
						while (((int8_t*)this->block + (temp_ref->shift() + size) > this->block_end) && try_count < 5) {
							this->allocate_new_block();
							try_count++;
						}
						temp_ref->ref_count()++;
						next_ref = this->reference_table.emplace_after(temp_ref, _ref());
						next_ref->shift() = temp_ref->shift() + size;

						if (SimpleMathApp::TestLog::TEST()) {
							SimpleMathApp::TestLog::LOG_STREAM() << "Relative shift: "<< temp_ref->shift()<<", size allocated: "<< size << std::endl;
						}

						return temp_ref->get_ref_ptr();
					}
					else if (!next_ref->shift()) {
						uint32_t try_count = 0;
						while (((int8_t*)this->block + (temp_ref->shift() + size) > this->block_end) && try_count < 5) {
							this->allocate_new_block();
							try_count++;
						}
						temp_ref->ref_count()++;
						next_ref->shift() = temp_ref->shift() + size; //задать конец куска памяти/начало следующего.

						if (SimpleMathApp::TestLog::TEST()) {
							SimpleMathApp::TestLog::LOG_STREAM() << "Relative shift: " << temp_ref->shift() << ", size allocated: " << size << std::endl;
						}

						return temp_ref->get_ref_ptr();

					}
					else {//next_ref->shift != 0
						if (temp_ref->shift() + size <= next_ref->shift()) {//достаточно ли места
							temp_ref->ref_count() ++;
							if (!next_ref->ref_count()) {//если на следующий блок памяти ничего не указывает - передвинуть указатель поближе (ради более плотной упаковки)
								next_ref->shift() = temp_ref->shift() + size;
							}
							else {
								//TODO:дополнять таблицу адресов новым элементом, стоящим между temp_ref и next_ref  (ради более плотной упаковки)
							}

							if (SimpleMathApp::TestLog::TEST()) {
								SimpleMathApp::TestLog::LOG_STREAM() << "Relative shift: " << temp_ref->shift() << ", size allocated: " << size << std::endl;
							}

							return temp_ref->get_ref_ptr();
						}
					}
				}
				else {//первая запись в таблице
					temp_ref->shift() = 0;
					temp_ref->ref_count() ++;
					next_ref->shift() = temp_ref->shift() + size;

					if (SimpleMathApp::TestLog::TEST()) {
						SimpleMathApp::TestLog::LOG_STREAM() << "Relative shift: " << temp_ref->shift() << ", size allocated: " << size << std::endl;
					}

					return temp_ref->get_ref_ptr();
				}
			}
		}
		return nullptr;//на всякий случай.
	}

	_ref * _memory_interface::relocate_mem(size_t _size_of_type, size_t _new_size_of_arr, ref_iterator _ref_before)
	{		

		size_t size = _size_of_type*_new_size_of_arr;		
		uint32_t last_shift_buf = 0;
		ref_iterator before_ref = _ref_before;
		ref_iterator temp_ref = _ref_before;
		temp_ref++;
		_ref buf = *(_ref_before->get_ref_ptr());
		ref_iterator next_ref = temp_ref;
		next_ref++;
		size_t size_before = next_ref->shift() - temp_ref->shift();

		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << "Call for _memory_interface::relocate_mem(...). Old relative shift: " << temp_ref->shift() << ", size: " << size_before << std::endl;
		}

		while (next_ref != this->reference_table.end()) {
			if (!temp_ref->ref_count())
				if (temp_ref->shift() + size <= next_ref->shift())break;
			before_ref++;
			temp_ref++;
			next_ref++;
		}
		last_shift_buf = temp_ref->shift();
		_ref buf_n = *(_ref_before->get_ref_ptr());
		this->reference_table.erase_after(before_ref);
		this->reference_table.erase_after(_ref_before);
		temp_ref = this->reference_table.emplace_after(before_ref, buf);
		before_ref = this->reference_table.emplace_after(_ref_before, _ref());		
		temp_ref->shift() = last_shift_buf;		
		before_ref->shift() = buf.shift();
		if (next_ref == this->reference_table.end()) {			
			uint32_t try_count = 0;
			while (((int8_t*)this->block + (temp_ref->shift() + size) > this->block_end) && try_count < 5) {
				this->allocate_new_block();
				try_count++;
			}
			next_ref = this->reference_table.emplace_after(temp_ref, _ref());
		}	
		if (!next_ref->shift()) 
			next_ref->shift() = temp_ref->shift() + size;
		void *src = ((int8_t*)this->block + before_ref->shift());
		void *dest = ((int8_t*)this->block + temp_ref->shift());
		std::memcpy(dest, src, size_before);
		
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << "New relative shift: " << temp_ref->shift() << ", size allocated: " << size << std::endl;
		}	

		return temp_ref->get_ref_ptr();
	}

	MemoryControl::ref_iterator _memory_interface::get_iterator_before(_ref * _ref)
	{
		ref_iterator out = this->reference_table.before_begin();
		ref_iterator next_ref = out;
		next_ref++;
		while (next_ref!= this->reference_table.end()&&next_ref->get_ref_ptr() != _ref) {
			out = next_ref;
			next_ref++;
		}
		return next_ref == this->reference_table.end()? next_ref:out;
	}

	MemoryControl::_memory_interface::_memory_interface():reference_table(),block(nullptr), block_end(nullptr)
	{		
		uint32_t try_count = 0;

		//WriteProcessMemory(hDF, (void*)Inject_point, modsrc, 13, NULL);
		//ReadProcessMemory(hDF, (void*)PauseStateAddr, &PauseState, 1, NULL);
		do {
#ifdef _WINDOWS_
			block = VirtualAllocEx(GetCurrentProcess(), NULL, init_memory() * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else//!_WINDOWS_	
			block = calloc(init_memory(), 1024);
#endif //_WINDOWS_			
			try_count++;
		} while (!block&&try_count < 5);
		if (!block) { 
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << "Failed to allocate memory pool." << std::endl;
			}
			throw std::bad_alloc();			
		}
		block_end = (void*)((int8_t*)block + init_memory() * 1024);
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << "Memory pool allocated at " << block << " from " << try_count << " try. Block ends at " << block_end << std::endl << std::endl;
		}
	}

	MemoryControl::_memory_interface::~_memory_interface()
	{
		//TODO:проверка счётчика ссылок. При завершении работы программы в принципе не требуется. но возможно потребуется переинициализация пула памяти в течение работы, например если реализовать сохранение/загрузку программ
		
		if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << std::endl << "Memory pool deallocated at " << block << std::endl;
			if (!this->reference_table.empty()) {
				ref_iterator iter = this->reference_table.begin();				
				uint32_t general_ref_count = 0;
				uint32_t size_of_adress_table = 0;
				int count = 0;
				while (iter != this->reference_table.end()) {					
					for (count = 1; count < iter->sizeof_counter_array(); count++) {
						general_ref_count += iter->ref_count(count);
					}
					size_of_adress_table++;
					iter++;
				}
				SimpleMathApp::TestLog::LOG_STREAM() << "Amount of external references left: " << general_ref_count << std::endl;
				SimpleMathApp::TestLog::LOG_STREAM() << "Amount of reference table records used: " << size_of_adress_table-1 << std::endl;
			}
		}

#ifdef _WINDOWS_
		VirtualFreeEx(GetCurrentProcess(), this->block, 0, MEM_RELEASE);
#else//!_WINDOWS_	
		free(block);
#endif//_WINDOWS_	
		this->reference_table.~forward_list();
	}

	std::forward_list<_ref>& _memory_interface::get_ref_table()
	{
		return this->reference_table;
	}

	void * _memory_interface::get_block()
	{
		return this->block;
	}

	void * _memory_interface::get_block_end()
	{
		return this->block_end;
	}



	//_ref

	void _ref::init()
	{
		int try_count = 0;
		do {
			//arr_count = (uint32_t*)new uint32_t[2];
			arr_count = (uint32_t*)calloc(size, sizeof(uint32_t));
			if (arr_count) { 
				temp_count = &arr_count[1];
				break;
			}
			try_count++;
		} while (!arr_count&&try_count<5);		
	}

	_ref * _ref::get_ref_ptr()
	{
		return this;
	}

	_ref::_ref() throw(...) :size(2), arr_count(nullptr)
	{		
		this->init();
		if (!arr_count) {
			throw std::bad_alloc();
		}		
	}

	_ref::_ref(const _ref & _original) throw(...):size(_original.size),arr_count(nullptr)
	{
		this->init();
		if (!arr_count) {
			throw std::bad_alloc();
		}
		memcpy(arr_count, _original.arr_count, size * sizeof(uint32_t));		
	}

	_ref::_ref(const _ref && _original) :size(_original.size), arr_count(_original.arr_count)
	{
		this->init();
		if (!arr_count) {
			throw std::bad_alloc();
		}
		memcpy(arr_count, _original.arr_count, size * sizeof(uint32_t));		
	}

	_ref::~_ref()
	{
		//delete[]arr_count;
		free(arr_count);
	}

	uint32_t& _ref::shift()
	{
		return arr_count[0];
	}

	uint32_t & _ref::ref_count(uint16_t _index)
	{
		if (_index >= size) return arr_count[1];
		return arr_count[_index];		
	}

	void _ref::alloc_more_count(uint16_t _plus)throw(...)
	{		
		/*if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << "Call for _ref::alloc_more_count(...). Old size of counter array: " << size << std::endl;
		}*/
		uint16_t new_size = size + _plus;
		int try_count = 0;
		uint32_t* new_count = nullptr;
		do {
			//new_count = (uint32_t*)new uint32_t[new_size];
			new_count = (uint32_t*)calloc(new_size, sizeof(uint32_t));
			if (new_count) break;
			try_count++;
		} while (!new_count&&try_count<5);
		if (!new_count) {
			if (SimpleMathApp::TestLog::TEST()) {
				SimpleMathApp::TestLog::LOG_STREAM() << "Failed to reallocate counter array." << std::endl;
			}
			throw std::bad_alloc();
		}
		for (uint16_t i = 0; i < size; i++)
			new_count[i] = arr_count[i];
		size = new_size;
		//delete[] arr_count;
		free(arr_count);
		arr_count = new_count;
		temp_count = &arr_count[1];
		/*if (SimpleMathApp::TestLog::TEST()) {
			SimpleMathApp::TestLog::LOG_STREAM() << "New size of counter array: " << new_size << std::endl;
		}*/
	}

	uint16_t _ref::find_zero_count()
	{
		if (size == 2) {
			alloc_more_count();
			return 2;
		}		
		for (uint16_t count = 2; count < size; count++) {
			if (!arr_count[count]) return count;
		}
		uint16_t out = size;
		alloc_more_count();
		return out;		
	}

	_ref & _ref::operator=(const _ref & _right)throw(...)
	{
		if (this == &_right) return *this;
		if(arr_count)
			free(arr_count);
		arr_count = nullptr;
		size = _right.size;
		this->init();
		if (!arr_count) {
			throw std::bad_alloc();
		}
		memcpy(arr_count, _right.arr_count, size * sizeof(uint32_t));
		return *this;
	}

	uint16_t _ref::sizeof_counter_array()
	{
		return this->size;
	}
	
	_memory_interface & InitializeMemPool()
	{
		return *MemPool();
	}

	_memory_interface & ReinitializeMemPool()
	{
		_memory_interface* prev = MemPool();
		prev->~_memory_interface();
		prev = new(prev) _memory_interface();
		return *MemPool();
	}

	void DestMemPool()
	{
		_memory_interface* prev = MemPool();		
		delete prev;
	}

	size_t ReadInitialfromFile()
	{		
		SimpleMathApp::Settings::SettingsFileHandler file;		
		size_t out = file.ReadInitialMemorysize();
		return out;
	}
}


