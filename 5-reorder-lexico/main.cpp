#include "../shared/bit_files.hpp"
#include "../shared/analysis.hpp"
#include "../shared/sorting.hpp"
#include <time.h>

void sort_lexicographically(BitTableView& bit_table_view);

int main(int argc, const char** argv) {

  if (argc != 3) {
    printf("usage: reorder-lexico <bit-file-in> <bit-file-out>\n"
           "\n"
           "  <bit-file-in>  The bit file (.txt) that you want to reorder.\n"
           "  <bit-file-out> The file name of the resulting ordered bit file.\n"
           "\n"
           "example: reorder-lexico uniform.txt uniform_ordered.txt\n"
           "\n");
    return 0;
  }
  
  clock_t t, t0;

  printf("Loading bit file...\n");
  t = t0 = clock();

  auto table = read_bit_file(argv[1]);
  BitTableView table_view(table);

  printf("[%.4fs] ", (float)(clock() - t)/CLOCKS_PER_SEC);
  printf("Loaded: %d columns, %d rows.\n", table.num_cols, table.num_rows);

  printf("Sorting rows lexicographically...\n");
  t = clock();

  sort_lexicographically(table_view);

  printf("[%.4fs] ", (float)(clock() - t)/CLOCKS_PER_SEC);
  printf("Sorted!\n");

  printf("Saving bit file...\n");
  t = clock();

  write_bit_file(argv[2], table_view);

  printf("[%.4fs] ", (float)(clock() - t)/CLOCKS_PER_SEC);
  printf("Saved!\n");

  printf("\nTotal time: [%.4fs]\n", (float)(clock() - t0)/CLOCKS_PER_SEC);

  return 0;
}
