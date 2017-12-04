#include <cmath>
#include "../shared/profiling.hpp"
#include "../shared/bit_files.hpp"
#include "visualisation.hpp"

int main(int argc, const char** argv) {

  if (argc != 5) {
    printf("usage: visualise <bit-file> <prob-1-bmp> <prob-2-bmp> <skew-bmp>\n"
           "\n"
           "  <bit-file>    The bit file (.txt) that you want to visualise.\n"
           "  <prob-1-bmp>  The file name of the first bitmap file to output.\n"
           "  <prob-2-bmp>  The file name of the second bitmap file to output.\n"
           "  <skew-bmp>    The file name of the skewness bitmap file to outpu.\n"
           "\n"
           "example: visualise uniform.txt uniform1.bmp uniform2.bmp uniform3.bmp\n"
           "\n");
    return 0;
  }
  
  auto table = read_bit_file(argv[1]);
  BitTableView table_view(table);
  
  int sample_space = 400000;
  sample_space = (table.num_rows < sample_space) ? table.num_rows : sample_space;

  auto sample_view = table_view.make_sub_view_of_rows(0, sample_space);

  auto sums = sum_by_columns(sample_view);
  auto cross_sums_1 = cross_sum_columns(sample_view);
  auto cross_sums_2 = cross_sum_columns_inverse(sample_view);

  auto prob_table_1 = generate_probability_table(sample_view, sums, cross_sums_1);
  auto prob_table_2 = generate_probability_table_inverse(sample_view, sums, cross_sums_2);
  auto skew_table = generate_skewness_table(sample_view, sums, cross_sums_1, cross_sums_2);

  auto image_1 = visualise_probability_table(table.num_cols, prob_table_1);
  write_bitmap_image(argv[2], image_1);

  auto image_2 = visualise_probability_table(table.num_cols, prob_table_2);
  write_bitmap_image(argv[3], image_2);

  auto image_3 = visualise_skewness_table(table.num_cols, skew_table);
  write_bitmap_image(argv[4], image_3);

  return 0;
}
