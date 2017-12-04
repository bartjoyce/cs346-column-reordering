#ifndef BitmapImage_hpp
#define BitmapImage_hpp

#include "colors.hpp"
#include <string.h>

class BitmapImage {
public:
  BitmapImage(int width, int height) {
    this->width = width;
    this->height = height;
    data = new char[3 * width * height];
    memset(&data[0], 255, 3 * width * height);
  }
  ~BitmapImage() {
    delete[] data;
  }

  void set_pixel(int x, int y, RGB color) {
    data[(x + y * width) * 3 + 2] = (unsigned char)(color.r * 255);
    data[(x + y * width) * 3 + 1] = (unsigned char)(color.g * 255);;
    data[(x + y * width) * 3 + 0] = (unsigned char)(color.b * 255);;
  }
  void set_pixel(int x, int y, HSV color) {
    set_pixel(x, y, HSV_to_RGB(color));
  }

  int width;
  int height;
  char* data;
};

void write_bitmap_image(const char* file_name, BitmapImage& image);

#endif
