#include "BitmapImage.hpp"
#include <stdio.h>

void write_bitmap_image(const char* file_name, BitmapImage& image) {
  int width = image.width;
  int height = image.height;

  int filesize = 54 + 3 * width * height;

  unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
  unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
  unsigned char bmppad[3] = {0,0,0};

  bmpfileheader[ 2] = (unsigned char)(filesize    );
  bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
  bmpfileheader[ 4] = (unsigned char)(filesize>>16);
  bmpfileheader[ 5] = (unsigned char)(filesize>>24);

  bmpinfoheader[ 4] = (unsigned char)(width    );
  bmpinfoheader[ 5] = (unsigned char)(width>> 8);
  bmpinfoheader[ 6] = (unsigned char)(width>>16);
  bmpinfoheader[ 7] = (unsigned char)(width>>24);
  bmpinfoheader[ 8] = (unsigned char)(height    );
  bmpinfoheader[ 9] = (unsigned char)(height>> 8);
  bmpinfoheader[10] = (unsigned char)(height>>16);
  bmpinfoheader[11] = (unsigned char)(height>>24);

  FILE* fp = fopen(file_name, "wb");
  if (fp == NULL) {
    printf("Output file couldn't be opened.\n");
    throw 0;
  }
  
  fwrite(bmpfileheader, 1, 14, fp);
  fwrite(bmpinfoheader, 1, 40, fp);
  for (int y = 0; y < height; ++y) {
    fwrite(image.data + (width * (height - y - 1) * 3), 3, width, fp);
    fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, fp);
  }
  
  fclose(fp);
}
