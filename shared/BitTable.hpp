#ifndef BitTable_hpp
#define BitTable_hpp

#include <cmath>
#include <stdio.h>
#include <string.h>

struct BitTable {
  BitTable(int num_rows, int num_cols) {
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    cols_capacity = (int)(std::ceil((double)num_cols / 16.0) * 16.0);
    data = new char[num_rows * cols_capacity];

    column_indices = new int[num_cols];
    for (int col = 0; col < num_cols; ++col) {
      column_indices[col] = col;
    }
  }
  ~BitTable() {
    delete[] data;
    delete[] column_indices;
  }

  char* operator[](int row_index) {
    return &data[row_index * cols_capacity];
  }

  int num_rows;
  int num_cols;
  int cols_capacity;
  int* column_indices;
  char* data;
};

#endif
