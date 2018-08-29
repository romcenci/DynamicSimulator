#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <GLFW/glfw3.h>

typedef struct {
    double h, s, v;
} HSV_Color;

typedef struct {
  double r, g, b;
} RGB_Color;

struct Color{
  double r, g, b;
} *pal;

int readColorFile(char *optarg){
  int ch,i;
  FILE *f1=fopen(optarg,"r");

  int NCOLORS=0;
  while(!feof(f1)){ // Conta o número de linhas
    ch = fgetc(f1);
    if(ch == '\n'){
      NCOLORS++;
    }
  }

  pal=malloc(NCOLORS*sizeof(struct Color));

  char hex[6];
  int r,g,b;
  rewind(f1);
  for(i=0; i<NCOLORS; i++){
    fscanf(f1,"%*2s%s",hex);
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    pal[i].r=r/255.; pal[i].g=g/255.; pal[i].b=b/255.;
  }
  fclose(f1);

  return NCOLORS;
}


double min(double num1,double num2){
  if (num1 <= num2)
    return num1;
  else
    return num2;
}

double max(double num1,double num2){
  if (num1 >= num2)
    return num1;
  else
    return num2;
}

void RGB2HSV(RGB_Color *rgb_pal, HSV_Color *hsv_pal, int ncolors){
  double min_, max_, d;
  for (int i = 0; i < ncolors; i++) {
    max_ = max(max(rgb_pal[i].r,rgb_pal[i].g),rgb_pal[i].b);
    min_ = min(min(rgb_pal[i].r,rgb_pal[i].g),rgb_pal[i].b);
    hsv_pal[i].v = max_;
    if (max_ == 0)
      hsv_pal[i].s = 0.0;
    else
      hsv_pal[i].s = (max_-min_)/max_;

    if (hsv_pal[i].s == 0)
      hsv_pal[i].h = fabs(0.0/0.0);
    else{
      d = max_ - min_;
      if (rgb_pal[i].r == max_){
	hsv_pal[i].h = (rgb_pal[i].g - rgb_pal[i].b)/d;
      }
      else if (rgb_pal[i].g == max_){
	hsv_pal[i].h = 2.0 + (rgb_pal[i].b - rgb_pal[i].r)/d;
      }
      else if (rgb_pal[i].b == max_){
	hsv_pal[i].h = 4.0 + (rgb_pal[i].r - rgb_pal[i].g)/d;
      }
      hsv_pal[i].h = hsv_pal[i].h*60.0;
      if (hsv_pal[i].h < 0)
	hsv_pal[i].h = hsv_pal[i].h + 360.0;
    }
  }
}

void HSV2RGB(RGB_Color *rgb_pal, HSV_Color *hsv_pal, int ncolors){
  int sextant;
  double fract, p, q, t;
  double h,s,v;
  for(int i = 0; i < ncolors;i++){
    h = hsv_pal[i].h;
    s = hsv_pal[i].s;
    v = hsv_pal[i].v;
    if (s == 0){
      if (isnan(h)){rgb_pal[i].r = rgb_pal[i].g = rgb_pal[i].b = v;}
      else{
	printf("Error in HSV Palette");
	exit(EXIT_FAILURE);
      }
    }
    else{
      if (h == 360)
	h = 0;
      else
	h = h/60;
      sextant = (int) floor(h);
      fract = h - sextant;
      p = v*(1-s);
      q = v*(1-s*fract);
      t = v*(1-s*(1-fract));
      switch (sextant) {
      case 0:
	rgb_pal[i].r = v; rgb_pal[i].g = t; rgb_pal[i].b = p; break;
      case 1:
	rgb_pal[i].r = q; rgb_pal[i].g = v; rgb_pal[i].b = p; break;
      case 2:
	rgb_pal[i].r = p; rgb_pal[i].g = v; rgb_pal[i].b = t; break;
      case 3:
	rgb_pal[i].r = p; rgb_pal[i].g = q; rgb_pal[i].b = v; break;
      case 4:
	rgb_pal[i].r = t; rgb_pal[i].g = p; rgb_pal[i].b = v; break;
      case 5:
	rgb_pal[i].r = v; rgb_pal[i].g = p; rgb_pal[i].b = q; break;

      }
    }
  }
}
