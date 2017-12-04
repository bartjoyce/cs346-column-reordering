#ifndef main_hpp
#define main_hpp

#include <cmath>
#include <stdio.h>

// Words are represented as unsigned integers. Accessing individual rows is done
// with bit masks.
unsigned int BIT_MASK[32] = {
  2147483648, 1073741824, 536870912, 268435456, 134217728,
  67108864, 33554432, 16777216, 8388608, 4194304, 2097152,
  1048576, 524288, 262144, 131072, 65536, 32768, 16384, 8192,
  4096, 2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1
};

#define FULL_BIT_MASK 4294967295

struct Word {
  unsigned int data;

  bool get_bit(int index) {
    unsigned int mask = BIT_MASK[index];
    return ((data & mask) == mask);
  }
  void set_bit(int index, bool value) {
    unsigned int mask = BIT_MASK[index];
    data = (data & (mask ^ FULL_BIT_MASK)) | ((unsigned int)value * mask);
  }
};

// Bit files are represented as arrays of columns. Columns are represented
// as an array of words, each containing 31 rows.
struct Column {
  int num_rows;
  int num_words;
  Word* words;
};

struct BitFile {
  int num_cols;
  Column* columns;
};

struct RunCount {
  int number_of_runs;
  int total_run_lengths;
};

struct Compression {
  int original_bits;
  int compressed_bits;
};

BitFile read_bit_file(const char* file_name);
RunCount count_runs(Column column);
Compression calculate_compression(Column column, RunCount run_count);

#define RUN_OF_ZERO 0
#define RUN_OF_ONE 2147483647

#define STATE_NO_RUN 0
#define STATE_RUN_OF_ZERO 1
#define STATE_RUN_OF_ONE 2

#endif
