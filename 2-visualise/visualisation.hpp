#ifndef visualisation_hpp
#define visualisation_hpp

#include "BitmapImage.hpp"
#include "../shared/BitTable.hpp"

BitmapImage visualise_probability_table(int num_cols, double* probability_table);
BitmapImage visualise_skewness_table(int num_cols, double* skewness_table);

#endif
