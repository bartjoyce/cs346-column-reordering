#include "bit_files.hpp"
#include <stdio.h>

BitTable read_bit_file(const char* file_name) {
  FILE* fp = fopen(file_name, "r");
  if (fp == NULL) {
    printf("Input file couldn't be opened\n");
    throw 0;
  }
  
  // Figure out the total file size.
  fseek(fp, 0, SEEK_END);
  int fsize = (int)ftell(fp);
  fseek(fp, 0, SEEK_SET);
  
  // Figure out the column size and capacity by reading
  // the first row.
  char chr;
  int cols_size = 0;
  while ((chr = fgetc(fp)) == '0' || chr == '1') {
    cols_size++;
  }
  
  int cols_capacity = cols_size + 1;
  while ((chr = fgetc(fp)) != '0' && chr != '1') {
    cols_capacity++;
  }
  
  // Jump back to the start.
  // Let's read the entire file!
  fseek(fp, 0, SEEK_SET);
  
  int rows = fsize / cols_capacity;
  
  BitTable table(rows, cols_size);

  char row_buffer[cols_capacity];

  for (int row = 0; row < rows; ++row) {
    fread(row_buffer, 1, cols_capacity, fp);
    char* row_in_table = table[row];
    for (int col = 0; col < cols_size; ++col) {
      row_in_table[col] = row_buffer[col] - '0';
    }
  }
  
  fclose(fp);
  return table;
}

void write_bit_file(const char* file_name, BitTableView& bit_table_view) {
  FILE* fp = fopen(file_name, "w");
  if (fp == NULL) {
    printf("Output file couldn't be opened.\n");
    throw 0;
  }
  
  char row_buffer[bit_table_view.num_cols + 1];
  row_buffer[bit_table_view.num_cols] = '\n';
  
  for (int row = 0; row < bit_table_view.num_rows; ++row) {
    for (int col = 0; col < bit_table_view.num_cols; ++col) {
      row_buffer[col] = bit_table_view.cell(row, col) + '0';
    }
    fwrite(row_buffer, 1, bit_table_view.num_cols + 1, fp);
  }
  
  fclose(fp);
}
