#include "main.hpp"

int main(int argc, const char** argv) {

  if (argc != 2) {
    printf("usage: eval <bit-file>\n"
           "\n"
           "  <bit-file>    The bit file (.txt) that you want to visualise.\n"
           "\n"
           "example: eval uniform.txt\n"
           "\n");
    return 0;
  }

  if (argc != 2) {
    printf("Expects 1 argument.\n");
    return 0;
  }

  BitFile file = read_bit_file(argv[1]);

  printf("Column, Number of Runs, Total Run-lengths, Uncompressed Bits, Compressed Bits, Compression Ratio\n");

  RunCount total_count;
  total_count.number_of_runs = 0;
  total_count.total_run_lengths = 0;

  Compression total_comp;
  total_comp.original_bits = 0;
  total_comp.compressed_bits = 0;

  for (int col = 0; col < file.num_cols; ++col) {
    RunCount count = count_runs(file.columns[col]);
    Compression comp = calculate_compression(file.columns[col], count);

    printf("%5d,%7d,%7d,%8d,%8d, %6.2f%%\n",
           col, count.number_of_runs, count.total_run_lengths,
           comp.original_bits, comp.compressed_bits,
           100.0 * (double)comp.compressed_bits / (double)comp.original_bits);

    total_count.number_of_runs += count.number_of_runs;
    total_count.total_run_lengths += count.total_run_lengths;

    total_comp.original_bits += comp.original_bits;
    total_comp.compressed_bits += comp.compressed_bits;
  }

  printf("TOTAL,%7d,%7d,%8d,%8d, %6.2f%%\n",
         total_count.number_of_runs, total_count.total_run_lengths,
         total_comp.original_bits, total_comp.compressed_bits,
         100.0 * (double)total_comp.compressed_bits / (double)total_comp.original_bits);

  return 0;
}

BitFile read_bit_file(const char* file_name) {
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
  int num_cols = 0;
  while ((chr = fgetc(fp)) == '0' || chr == '1') {
    num_cols++;
  }
  
  int num_chars_per_line = num_cols + 1;
  while ((chr = fgetc(fp)) != '0' && chr != '1') {
    num_chars_per_line++;
  }
  
  int num_rows = fsize / num_chars_per_line;
  int num_words = (int)std::ceil((double)num_rows / 31.0);

  // Allocate space in memory to represent the bit file.
  BitFile file;
  file.num_cols = num_cols;
  file.columns = new Column[num_cols];
  for (int col = 0; col < num_cols; ++col) {
    Column column;
    column.num_rows = num_rows;
    column.num_words = num_words;
    column.words = new Word[num_words];
    file.columns[col] = column;
  }

  // Jump back to the start.
  // Let's read the entire file!
  fseek(fp, 0, SEEK_SET);

  char row_buffer[num_chars_per_line];

  for (int row = 0; row < num_rows; ++row) {
    fread(row_buffer, 1, num_chars_per_line, fp);

    int word = row / 31;
    int bit = row % 31 + 1;

    for (int col = 0; col < num_cols; ++col) {
      file.columns[col].words[word].set_bit(bit, row_buffer[col] == '1');
    }
  }
  
  fclose(fp);
  return file;
}

RunCount count_runs(Column column) {
  int number_of_runs = 0;
  int total_run_lengths = 0;

  int state = STATE_NO_RUN;

  for (int word = 0; word < column.num_words; ++word) {
    unsigned int current_word = column.words[word].data;

    switch (state) {
      case STATE_NO_RUN:
        if (current_word == RUN_OF_ZERO) {
          number_of_runs++;
          total_run_lengths++;
          state = STATE_RUN_OF_ZERO;
        } else if (current_word == RUN_OF_ONE) {
          number_of_runs++;
          total_run_lengths++;
          state = STATE_RUN_OF_ONE;
        }
        break;

      case STATE_RUN_OF_ZERO:
        if (current_word == RUN_OF_ZERO) {
          total_run_lengths++;
        } else if (current_word == RUN_OF_ONE) {
          number_of_runs++;
          total_run_lengths++;
          state = STATE_RUN_OF_ONE;
        } else {
          state = STATE_NO_RUN;
        }
        break;

      case STATE_RUN_OF_ONE:
        if (current_word == RUN_OF_ZERO) {
          number_of_runs++;
          total_run_lengths++;
          state = STATE_RUN_OF_ZERO;
        } else if (current_word == RUN_OF_ONE) {
          total_run_lengths++;
        } else {
          state = STATE_NO_RUN;
        }
        break;
    }
  }

  RunCount result;
  result.number_of_runs = number_of_runs;
  result.total_run_lengths = total_run_lengths;
  return result;
}

Compression calculate_compression(Column column, RunCount run_count) {
  Compression result;
  result.original_bits = column.num_rows;
  result.compressed_bits = (column.num_words + run_count.number_of_runs - run_count.total_run_lengths) * 32;
  return result;
}
