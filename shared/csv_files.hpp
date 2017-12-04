#ifndef csv_files_hpp
#define csv_files_hpp

#include "BitTable.hpp"

void write_csv_sliding_window_file(const char* file_name, BitTable& sliding_window);
void write_csv_sums_file(const char* file_name, int num_cols, int* sums, int* cross_sums);
void write_csv_probabilities_file(const char* file_name, int num_cols, double* table);

#endif
