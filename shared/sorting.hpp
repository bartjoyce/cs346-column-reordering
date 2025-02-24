#ifndef sorting_hpp
#define sorting_hpp

#include "BitTableView.hpp"

void sort_column(BitTableView& bit_table_view, int column);
void sort_lexicographically(BitTableView& bit_table_view);
void sort_mutually_exclusive_group(BitTableView& bit_table_view);
int* count_mutually_exclusive_group(BitTableView& bit_table_view);

#endif
