#include "sorting.hpp"

void sort_column(BitTableView& bit_table_view, int column) {

  char* buffer = new char[bit_table_view.num_rows * bit_table_view.cols_capacity];

  int lower_bound = 0;
  int upper_bound = bit_table_view.num_rows - 1;
  
  // Write into the buffer sorted.
  for (int row = 0; row < bit_table_view.num_rows; ++row) {
    if (bit_table_view.cell(row, column) == 1) {
      memcpy(buffer + (upper_bound--) * bit_table_view.cols_capacity,
             bit_table_view.data + row * bit_table_view.cols_capacity,
             bit_table_view.cols_capacity);
    } else {
      memcpy(buffer + (lower_bound++) * bit_table_view.cols_capacity,
             bit_table_view.data + row * bit_table_view.cols_capacity,
             bit_table_view.cols_capacity);
    }
  }
  
  // Write buffer back to the table.
  for (int row = 0; row < lower_bound; ++row) {
    memcpy(bit_table_view.data + row * bit_table_view.cols_capacity,
           buffer + row * bit_table_view.cols_capacity,
           bit_table_view.cols_capacity);
  }
  for (int row = lower_bound; row < bit_table_view.num_rows; ++row) {
    memcpy(bit_table_view.data + row * bit_table_view.cols_capacity,
           buffer + (bit_table_view.num_rows - row + lower_bound - 1) * bit_table_view.cols_capacity,
           bit_table_view.cols_capacity);
  }
  
  delete[] buffer;

}

void sort_lexicographically(BitTableView& bit_table_view) {

  for (int col = bit_table_view.num_cols - 1; col >= 0; --col) {
    sort_column(bit_table_view, col);
  }

}

void sort_mutually_exclusive_group(BitTableView& bit_table_view) {
  int num_rows = bit_table_view.num_rows;
  int num_cols = bit_table_view.num_cols;
  int cols_capacity = bit_table_view.cols_capacity;

  // Dynamically scaling data is inefficient. Instead count the sections before-hand.
  auto counts = count_mutually_exclusive_group(bit_table_view);

  int* sections = new int[num_cols + 1];
  sections[0] = 0;
  for (int i = 1; i < num_cols + 1; ++i) {
    sections[i] = sections[i - 1] + counts[i - 1];
  }
  
  delete[] counts;

  // Sort all the rows into a buffer.
  char* buffer = new char[num_rows * cols_capacity];

  for (int row = 0; row < num_rows; ++row) {

    int sector = num_cols;
    for (int col = 0; col < num_cols; ++col) {
      if (bit_table_view.cell(row, col)) {
        sector = col;
        break;
      }
    }

    int row_out = sections[sector]++;
    memcpy(buffer + row_out * cols_capacity, bit_table_view.data + row * cols_capacity, cols_capacity);
  }

  // Copy the buffer back in to the original table,
  // reversing odd sections.
  {
    int lower_bound = 0;
    int upper_bound = sections[0];

    for (int row = lower_bound; row < upper_bound; ++row) {
      memcpy(bit_table_view.data + row * cols_capacity, buffer + row * cols_capacity, cols_capacity);
    }
  }
  for (int sector = 0; sector < num_cols; ++sector) {
    int lower_bound = sections[sector];
    int upper_bound = sections[sector + 1];

    if (sector % 2 == 0) {
      for (int row = lower_bound; row < upper_bound; ++row) {
        memcpy(bit_table_view.data + row * cols_capacity, buffer + row * cols_capacity, cols_capacity);
      }
    } else {
      int N = upper_bound - lower_bound;

      for (int i = 0; i < N; ++i) {
        memcpy(bit_table_view.data + (i + lower_bound) * cols_capacity,
               buffer + (upper_bound - i - 1) * cols_capacity, cols_capacity);
      }
    }
  }

  delete[] buffer;
  delete[] sections;
}

int* count_mutually_exclusive_group(BitTableView& bit_table_view) {
  int num_rows = bit_table_view.num_rows;
  int num_cols = bit_table_view.num_cols;

  int* counts = new int[num_cols + 1];
  memset(counts, 0, sizeof(int) * (num_cols + 1));
  counts[num_cols] = num_rows;

  for (int row = 0; row < num_rows; ++row) {
    for (int col = 0; col < num_cols; ++col) {
      if (bit_table_view.cell(row, col)) {
        ++counts[col];
        --counts[num_cols];
        break;
      }
    }
  }

  return counts;
}
