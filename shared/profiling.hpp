#ifndef profiling_hpp
#define profiling_hpp

#include "BitTableView.hpp"

int* sum_by_columns(BitTableView& bit_table_view);
int* cross_sum_columns(BitTableView& bit_table_view);
int* cross_sum_columns_inverse(BitTableView& bit_table_view);
double* generate_probability_table(BitTableView& bit_table_view, int* sums, int* cross_sums);
double* generate_probability_table_inverse(BitTableView& bit_table_view, int* sums, int* cross_sums);
double* generate_skewness_table(BitTableView& bit_table_view, int* sums, int* cross_sums, int* cross_sums_inverse);

#endif
