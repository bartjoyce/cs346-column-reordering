#include "../shared/bit_files.hpp"
#include "../shared/analysis.hpp"

int main(int argc, const char** argv) {

  if (argc != 2) {
    printf("usage: analyse <bit-file>\n"
           "\n"
           "  <bit-file>    The bit file (.txt) that you want to analyse.\n"
           "\n"
           "example: analyse uniform.txt\n"
           "\n");
    return 0;
  }
  
  auto table = read_bit_file(argv[1]);
  BitTableView table_view(table);
  
  int sample_space = 400000;
  sample_space = (table.num_rows < sample_space) ? table.num_rows : sample_space;

  auto sample_view = table_view.make_sub_view_of_rows(0, sample_space);

  auto analysis = analyse_bit_table(sample_view);

  printf("Low Density Columns: ");
  if (analysis.low_density_columns.empty()) {
    printf("(none)");
  } else {
    printf("C%d", analysis.low_density_columns.front());
  }
  for (auto it = analysis.low_density_columns.begin() + 1;
            it < analysis.low_density_columns.end(); ++it) {
    printf(", C%d", *it);
  }
  printf("\n\n");

  printf("High Density Columns: ");
  if (analysis.high_density_columns.empty()) {
    printf("(none)");
  } else {
    printf("C%d", analysis.high_density_columns.front());
  }
  for (auto it = analysis.high_density_columns.begin() + 1;
            it < analysis.high_density_columns.end(); ++it) {
    printf(", C%d", *it);
  }
  printf("\n\n");
  
  printf("Mutually Exclusive Column Groups:\n");
  for (auto it = analysis.mutually_exclusive_groups.begin();
            it < analysis.mutually_exclusive_groups.end(); ++it) {
    printf("Group %ld: ", (it - analysis.mutually_exclusive_groups.begin()));
    printf("C%d", it->front());
    for (auto it2 = it->begin() + 1; it2 < it->end(); ++it2) {
      printf(", C%d", *it2);
    }
    printf("\n");
  }

  return 0;
}
