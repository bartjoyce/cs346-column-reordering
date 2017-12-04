#include "../shared/bit_files.hpp"
#include "../shared/analysis.hpp"
#include "sorting.hpp"
#include <time.h>
#include <algorithm>

bool group_size_comparison(std::vector<int>& i, std::vector<int>& j) {
  return (i.size() < j.size());
}

int main(int argc, const char** argv) {

  if (argc != 3) {
    printf("usage: reorder <bit-file-in> <bit-file-out>\n"
           "\n"
           "  <bit-file-in>  The bit file (.txt) that you want to reorder.\n"
           "  <bit-file-out> The file name of the resulting ordered bit file.\n"
           "\n"
           "example: reorder uniform.txt uniform_ordered.txt\n"
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
  
  int sample_space = 400000;
  sample_space = (table.num_rows < sample_space) ? table.num_rows : sample_space;

  printf("Analysing the bit file...\n");
  t = clock();

  auto sample_view = table_view.make_sub_view_of_rows(0, sample_space);
  auto analysis = analyse_bit_table(sample_view);

  printf("[%.4fs] ", (float)(clock() - t)/CLOCKS_PER_SEC);
  printf("Analysis: %d low density columns, %d high density columns, %d other column groups.\n",
         analysis.low_density_columns.size(),
         analysis.high_density_columns.size(),
         analysis.mutually_exclusive_groups.size());

  printf("Sorting rows; largest column group takes priority...\n");
  t = clock();

  std::sort(analysis.mutually_exclusive_groups.begin(),
            analysis.mutually_exclusive_groups.end(),
            group_size_comparison);

  for (auto it = analysis.mutually_exclusive_groups.begin();
            it < analysis.mutually_exclusive_groups.end(); ++it) {

    auto sub_view = table_view.make_sub_view_of_cols(*it);
    sort_mutually_exclusive_group(sub_view);

  }

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
