#include "profiling.hpp"

#include <stdio.h>
#include <string.h>
#include <cmath>

#ifdef __APPLE__
  #include <xmmintrin.h>
#else
  #include <immintrin.h>
#endif

int* sum_by_columns(BitTableView& bit_table_view) {

  int num_rows = bit_table_view.num_rows;
  int num_cols = bit_table_view.num_cols;
  int cols_capacity = bit_table_view.cols_capacity;
  int* shared_indices = bit_table_view.shared_column_indices;
  int* indices = bit_table_view.column_indices;

  int* raw_sums = new int[cols_capacity];
  memset(raw_sums, 0, sizeof(int) * cols_capacity);

  for (int row = 0; row < num_rows; ++row) {
    auto row_data = &bit_table_view.data[row * cols_capacity];
    for (int col = 0; col < cols_capacity; ++col) {
      raw_sums[col] += row_data[col];
    }
  }

  int* sums = new int[num_cols];

  for (int col = 0; col < num_cols; ++col) {
    sums[col] = raw_sums[shared_indices[indices[col]]];
  }

  delete[] raw_sums;

  return sums;

}

int* cross_sum_columns(BitTableView& bit_table_view) {

  int num_rows = bit_table_view.num_rows;
  int num_cols = bit_table_view.num_cols;
  int cols_capacity = bit_table_view.cols_capacity;
  int* shared_indices = bit_table_view.shared_column_indices;
  int* indices = bit_table_view.column_indices;

  int* cross_sums = new int[num_cols * num_cols];
  memset(cross_sums, 0, sizeof(int) * num_cols * num_cols);

  unsigned char* cross_sums_bytes = new unsigned char[cols_capacity * num_cols];
  memset(cross_sums_bytes, 0, cols_capacity * num_cols);

  for (int row = 0; row < num_rows; ++row) {

    if (row && row % 240 == 0) {
      for (int col_y = 0; col_y < num_cols; ++col_y) {
        int offset_1 = col_y * num_cols;
        int offset_2 = col_y * cols_capacity;
        for (int col_x = 0; col_x < num_cols; ++col_x) {
          cross_sums[offset_1 + col_x] += cross_sums_bytes[offset_2 + shared_indices[indices[col_x]]];
        }
      }
      memset(cross_sums_bytes, 0, cols_capacity * num_cols);
    }

    auto row_data = &bit_table_view.data[row * cols_capacity];

    for (int col_y = 0; col_y < num_cols; ++col_y) {

      int offset = col_y * cols_capacity;

      __m128i cell_x = _mm_set1_epi8(row_data[shared_indices[indices[col_y]]]);

      for (int col_x = 0; col_x < cols_capacity; col_x += 16) {
        _mm_store_si128(
          (__m128i*)(cross_sums_bytes + offset + col_x),
          _mm_add_epi8(
            _mm_load_si128((__m128i*)(cross_sums_bytes + offset + col_x)),
            _mm_and_si128(cell_x, _mm_load_si128((__m128i*)(row_data + col_x)))
          )
        );
      }
    }
  }

  delete[] cross_sums_bytes;

  return cross_sums;

}

int* cross_sum_columns_inverse(BitTableView& bit_table_view) {

  int num_rows = bit_table_view.num_rows;
  int num_cols = bit_table_view.num_cols;
  int cols_capacity = bit_table_view.cols_capacity;
  int* shared_indices = bit_table_view.shared_column_indices;
  int* indices = bit_table_view.column_indices;

  int* cross_sums = new int[num_cols * num_cols];
  memset(cross_sums, 0, sizeof(int) * num_cols * num_cols);

  unsigned char* cross_sums_bytes = new unsigned char[cols_capacity * num_cols];
  memset(cross_sums_bytes, 0, cols_capacity * num_cols);

  for (int row = 0; row < num_rows; ++row) {

    if (row && row % 240 == 0) {
      for (int col_y = 0; col_y < num_cols; ++col_y) {
        int offset_1 = col_y * num_cols;
        int offset_2 = col_y * cols_capacity;
        for (int col_x = 0; col_x < num_cols; ++col_x) {
          cross_sums[offset_1 + col_x] += cross_sums_bytes[offset_2 + shared_indices[indices[col_x]]];
        }
      }
      memset(cross_sums_bytes, 0, cols_capacity * num_cols);
    }

    auto row_data = &bit_table_view.data[row * cols_capacity];

    for (int col_y = 0; col_y < num_cols; ++col_y) {

      int offset = col_y * cols_capacity;

      __m128i cell_x = _mm_set1_epi8(row_data[shared_indices[indices[col_y]]] ? 0 : 1);

      for (int col_x = 0; col_x < cols_capacity; col_x += 16) {
        _mm_store_si128(
          (__m128i*)(cross_sums_bytes + offset + col_x),
          _mm_add_epi8(
            _mm_load_si128((__m128i*)(cross_sums_bytes + offset + col_x)),
            _mm_and_si128(cell_x, _mm_load_si128((__m128i*)(row_data + col_x)))
          )
        );
      }
    }
  }

  delete[] cross_sums_bytes;

  return cross_sums;

}

double* generate_probability_table(BitTableView& bit_table_view, int* sums, int* cross_sums) {

  int num_cols = bit_table_view.num_cols;

  double* table = new double[num_cols * num_cols];

  for (int col_y = 0; col_y < num_cols; ++col_y) {
    int total = sums[col_y];
    if (total == 0) {
      for (int col_x = 0; col_x < num_cols; ++col_x) {
        table[col_y * num_cols + col_x] = 0.5;
      }
    } else {
      for (int col_x = 0; col_x < num_cols; ++col_x) {
        table[col_y * num_cols + col_x] = (double)cross_sums[col_y * num_cols + col_x] / (double)total;
      }
    }
  }

  return table;

}

double* generate_probability_table_inverse(BitTableView& bit_table_view, int* sums, int* cross_sums) {

  int num_cols = bit_table_view.num_cols;
  int num_rows = bit_table_view.num_rows;

  double* table = new double[num_cols * num_cols];

  for (int col_y = 0; col_y < num_cols; ++col_y) {
    int total = num_rows - sums[col_y];
    if (total == 0) {
      for (int col_x = 0; col_x < num_cols; ++col_x) {
        table[col_y * num_cols + col_x] = 0.5;
      }
    } else {
      for (int col_x = 0; col_x < num_cols; ++col_x) {
        table[col_y * num_cols + col_x] = (double)cross_sums[col_y * num_cols + col_x] / (double)total;
      }
    }
  }

  return table;

}

double* generate_skewness_table(BitTableView& bit_table_view, int* sums, int* cross_sums, int* cross_sums_inverse) {

  int num_cols = bit_table_view.num_cols;

  double* table = new double[num_cols * num_cols];

  for (int col_y = 0; col_y < num_cols; ++col_y) {
    for (int col_x = 0; col_x < num_cols; ++col_x) {
      int sum = sums[col_x];
      double skewness;
      if (sum == 0) {
        skewness = 0.0;
      } else {
        int cross_sum = cross_sums[col_y * num_cols + col_x];
        int cross_sum_inverse = cross_sums_inverse[col_y * num_cols + col_x];
        skewness = ((double)(cross_sum - cross_sum_inverse) / (double)sum);
      }
      table[col_y * num_cols + col_x] = skewness;
    }
  }

  return table;

}
