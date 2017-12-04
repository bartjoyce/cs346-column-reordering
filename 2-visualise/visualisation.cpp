#include "visualisation.hpp"

BitmapImage visualise_probability_table(int num_cols, double* probability_table) {

  BitmapImage image(num_cols, num_cols);

  HSV color;
  color.s = 1.0;
  color.v = 1.0;

  for (int y = 0; y < num_cols; ++y) {
    for (int x = 0; x < num_cols; ++x) {
      color.h = probability_table[y * num_cols + x] * 270.0;
      image.set_pixel(x, num_cols - y - 1, color);
    }
  }

  return image;

}

BitmapImage visualise_skewness_table(int num_cols, double* skewness_table) {

  BitmapImage image(num_cols, num_cols);

  HSV color;
  color.s = 1.0;
  color.v = 1.0;

  for (int y = 0; y < num_cols; ++y) {
    for (int x = 0; x < num_cols; ++x) {
      color.h = skewness_table[y * num_cols + x] * 135.0 + 135.0;
      image.set_pixel(x, num_cols - y - 1, color);
    }
  }

  return image;

}
