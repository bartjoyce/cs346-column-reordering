#include "analysis.hpp"
#include "profiling.hpp"

// #ifdef __APPLE__
//   #include <xmmintrin.h>
// #else
//   #include <immintrin.h>
// #endif

Analysis analyse_bit_table(BitTableView& bit_table_view) {

  int num_cols = bit_table_view.num_cols;
  int num_rows = bit_table_view.num_rows;

  int* sums = sum_by_columns(bit_table_view);

  std::vector<int> low_density;
  std::vector<int> high_density;

  std::vector<int> normal_columns;

  for (int col = 0; col < num_cols; ++col) {
    auto density = (double)sums[col] / (double)num_rows;
    if (density < 0.005) {
      low_density.push_back(col);
    } else if (density > 0.995) {
      high_density.push_back(col);
    } else {
      normal_columns.push_back(col);
    }
  }

  // Make a new sample view excluding all high and low density columns
  // so that cross-summing is more efficient.
  BitTableView sample_view = bit_table_view;

  int num_normal_cols = normal_columns.size();
  sample_view.column_indices = &normal_columns[0];
  sample_view.num_cols = num_normal_cols;

  auto cross_sums = cross_sum_columns(sample_view);

  std::vector<std::vector<int> > mutually_exclusive_groups;

  // Greedily build cliques
  for (int col_y = 0; col_y < num_normal_cols; ++col_y) {

    std::vector<int>* contender_group = nullptr;

    for (auto group = mutually_exclusive_groups.begin();
              group < mutually_exclusive_groups.end(); ++group) {

      if (contender_group && contender_group->size() > group->size()) {
        continue; // Don't consider any groups smaller than the contender
      }

      bool compatible = true;
      for (auto col_x = group->begin(); col_x < group->end(); ++col_x) {
        if (cross_sums[col_y * num_normal_cols + *col_x] != 0) {
          compatible = false;
          break;
        }
      }

      if (compatible) {
        contender_group = &*group;
      }

    }

    if (contender_group) {
      contender_group->push_back(col_y);
    } else {
      std::vector<int> new_group;
      new_group.push_back(col_y);
      mutually_exclusive_groups.push_back(std::move(new_group));
    }

  }

  // Result!
  Analysis result;
  result.low_density_columns = std::move(low_density);
  result.high_density_columns = std::move(high_density);
  result.mutually_exclusive_groups = std::move(mutually_exclusive_groups);
  return result;

}
