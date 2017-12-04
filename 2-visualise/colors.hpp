#ifndef colors_hpp
#define colors_hpp

typedef struct {
  double r;       // a fraction between 0 and 1
  double g;       // a fraction between 0 and 1
  double b;       // a fraction between 0 and 1
} RGB;

typedef struct {
  double h;       // angle in degrees
  double s;       // a fraction between 0 and 1
  double v;       // a fraction between 0 and 1
} HSV;

HSV RGB_to_HSV(RGB in);
RGB HSV_to_RGB(HSV in);

#endif
