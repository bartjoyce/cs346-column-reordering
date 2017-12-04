#ifndef analysis_hpp
#define analysis_hpp

#include "BitTableView.hpp"
#include <vector>

struct Analysis {
  std::vector<int>  low_density_columns;
  std::vector<int> high_density_columns;
  std::vector<std::vector<int> > mutually_exclusive_groups;
};

Analysis analyse_bit_table(BitTableView& bit_table_view);

#endif
