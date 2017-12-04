#include "csv_files.hpp"
#include <stdio.h>
#include <cmath>

void write_csv_sliding_window_file(const char* file_name, BitTable& sliding_window) {
  int num_rows = sliding_window.num_rows;
  int num_cols = sliding_window.num_cols;

  FILE* fp = fopen(file_name, "w");
  if (fp == NULL) {
    printf("Output file couldn't be opened.\n");
    throw 0;
  }

  // CSV Header
  fputs("DIST", fp);
  for (int col = 0; col < num_cols; ++col) {
    fprintf(fp, ",+%d", col + 1);
  }
  fputc('\n', fp);

  // CROSS
  for (int row = 0; row < num_rows; ++row) {
    fprintf(fp, "R%d", row);
    for (int look_ahead = 0; look_ahead < num_cols; ++look_ahead) {
      fprintf(fp, ",%d", sliding_window[row][look_ahead]);
    }
    fputc('\n', fp);
  }
  
  fclose(fp);
}

void write_csv_sums_file(const char* file_name, int num_cols, int* sums, int* cross_sums) {
  FILE* fp = fopen(file_name, "w");
  if (fp == NULL) {
    printf("Output file couldn't be opened.\n");
    throw 0;
  }

  // CSV Header
  fputs("SUMS", fp);
  for (int col = 0; col < num_cols; ++col) {
    fprintf(fp, ",C%d", col);
  }
  fputc('\n', fp);

  // SUM
  fputs("SUM", fp);
  for (int col = 0; col < num_cols; ++col) {
    fprintf(fp, ",%d", sums[col]);
  }
  fputc('\n', fp);

  // CROSS
  for (int col_x = 0; col_x < num_cols; ++col_x) {
    fprintf(fp, "C%d", col_x);
    for (int col_y = 0; col_y < num_cols; ++col_y) {
      fprintf(fp, ",%d", cross_sums[col_x * num_cols + col_y]);
    }
    fputc('\n', fp);
  }
  
  fclose(fp);
}

void write_csv_probabilities_file(const char* file_name, int num_cols, double* table) {
  FILE* fp = fopen(file_name, "w");
  if (fp == NULL) {
    printf("Output file couldn't be opened.\n");
    throw 0;
  }

  // CSV Header
  fputs("P(CB=1|CA=1)", fp);
  for (int col = 0; col < num_cols; ++col) {
    fprintf(fp, ",B=%d", col);
  }
  fputc('\n', fp);

  // CROSS
  for (int col_x = 0; col_x < num_cols; ++col_x) {
    fprintf(fp, "A=%d", col_x);
    for (int col_y = 0; col_y < num_cols; ++col_y) {
      fprintf(fp, ",%d%%", (int)std::round(100.0 * table[col_x * num_cols + col_y]));
    }
    fputc('\n', fp);
  }
  
  fclose(fp);
}
