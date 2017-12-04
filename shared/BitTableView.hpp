#ifndef BitTableView_hpp
#define BitTableView_hpp

#include "BitTable.hpp"
#include <vector>

struct BitTableView {
  BitTableView() = default;
  BitTableView(BitTable& bit_table) {
    num_rows = bit_table.num_rows;
    num_cols = bit_table.num_cols;
    cols_capacity = bit_table.cols_capacity;
    shared_column_indices = bit_table.column_indices;
    data = bit_table.data;

    column_indices = new int[num_cols];
    for (int col = 0; col < num_cols; ++col) {
      column_indices[col] = col;
    }
  }

  void swap_rows(int row_a, int row_b) {
    char* row_a_data = &data[row_a * cols_capacity];
    char* row_b_data = &data[row_b * cols_capacity];
    char tmp_data[cols_capacity];

    memcpy(tmp_data, row_a_data, cols_capacity);
    memcpy(row_a_data, row_b_data, cols_capacity);
    memcpy(row_b_data, tmp_data, cols_capacity);
  }
  void swap_cols(int col_a, int col_b) {
    int real_col_a = column_indices[col_a];
    int real_col_b = column_indices[col_b];

    int tmp = shared_column_indices[real_col_a];
    shared_column_indices[real_col_a] = shared_column_indices[real_col_b];
    shared_column_indices[real_col_b] = tmp;
  }
  void swap_cols_virtually(int col_a, int col_b) {
    int tmp = column_indices[col_a];
    column_indices[col_a] = column_indices[col_b];
    column_indices[col_b] = tmp;
  }

  BitTableView make_sub_view_of_rows(int row_from, int row_to) {
    BitTableView sub_view;
    
    sub_view.num_rows = row_to - row_from;
    sub_view.num_cols = num_cols;
    sub_view.cols_capacity = cols_capacity;
    sub_view.shared_column_indices = shared_column_indices;
    sub_view.data = &data[row_from * cols_capacity];

    sub_view.column_indices = new int[num_cols];
    for (int col = 0; col < num_cols; ++col) {
      sub_view.column_indices[col] = column_indices[col];
    }

    return sub_view;
  }

  BitTableView make_sub_view_of_cols(int col_from, int col_to) {
    BitTableView sub_view;

    sub_view.num_rows = num_rows;
    sub_view.num_cols = col_to - col_from;
    sub_view.cols_capacity = cols_capacity;
    sub_view.shared_column_indices = shared_column_indices;
    sub_view.data = data;
    
    sub_view.column_indices = new int[sub_view.num_cols];
    for (int i = 0; i < sub_view.num_cols; ++i) {
      sub_view.column_indices[i] = column_indices[col_from + i];
    }

    return sub_view;
  }

  BitTableView make_sub_view_of_cols(std::vector<int>& cols) {
    BitTableView sub_view;

    sub_view.num_rows = num_rows;
    sub_view.num_cols = cols.size();
    sub_view.cols_capacity = cols_capacity;
    sub_view.shared_column_indices = shared_column_indices;
    sub_view.data = data;
    
    sub_view.column_indices = new int[sub_view.num_cols];
    for (int i = 0; i < sub_view.num_cols; ++i) {
      sub_view.column_indices[i] = column_indices[cols[i]];
    }

    return sub_view;
  }

  char& cell(int row, int col) {
    return data[row * cols_capacity + shared_column_indices[column_indices[col]]];
  }

  int num_rows;
  int num_cols;
  int cols_capacity;
  int* column_indices;
  int* shared_column_indices;
  char* data;
};

#endif
