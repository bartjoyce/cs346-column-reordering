#ifndef bit_files_hpp
#define bit_files_hpp

#include "BitTable.hpp"
#include "BitTableView.hpp"

BitTable read_bit_file(const char* file_name);
void write_bit_file(const char* file_name, BitTableView& bit_table_view);

#endif
