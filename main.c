 /*
  * gcc main.c -o mandel --std=c99 -O2 -lm -lX11
  */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>

#define maxiterations 200
#define width  640
#define height 480

double zoom   = 0.999;
float  angle  = 0.0;
double w = 5.0;
double h;
unsigned int cl_inside = 0xffffff;
unsigned int palette[maxiterations];

void setup() {
  h = (w * height) / width;
  //colorMode(HSB, 1);
  
  for (int i = 0; i < maxiterations; i++) {
    float inten = sqrt((float)i / maxiterations);
    palette[i] = inten * 0xffffff;
  }
}

void draw(XImage *image) {
  w *= zoom;
  h *= zoom;
  angle += 0.001;

  double C = cos(angle);
  double S = sin(angle);

  double xcenter = -w/2.0;
  double ycenter = -h/2.0;
  
  //double xcenter = -0.7453 - w/2.0;
  //double ycenter =  0.1127 - h/2.0;
  
  double xmin = xcenter * C - ycenter * S;
  double ymin = ycenter * C + xcenter * S;

  // Calculate amount we increment x,y for each pixel
  double horisontalX = C * w / (width);
  double horisontalY = S * w / (width);
  double verticalX   = -S * h / (height);
  double verticalY   = C * h / (height);
  
  unsigned int *pixels = (unsigned int *)image->data;
  for (int j = 0; j < height; j++) {
    double x = j * verticalX + xmin;
    double y = j * verticalY + ymin;
    for (int i = 0; i < width; i++) {
      
      double a = x -0.7453;
      double b = y +0.1127;
      int n = 0;
      while (n < maxiterations) {
        double aa = a * a;
        double bb = b * b;
        double twoab = a * b; twoab += twoab; // *+ instead of **
        a = aa - bb + x -0.7453;
        b = twoab + y +0.1127;

        if (aa + bb > 16.0) {
          break;
        }
        n++;
      }

      if (n == maxiterations) {
        pixels[i+j*width] = cl_inside;
      } else {
        pixels[i+j*width] = palette[n];
      }
      x += horisontalX;
      y += horisontalY;
    }
  }
  
  //println(frameRate);
}

int main(int argc, char **argv) {
    Display *display = XOpenDisplay(NULL);
    Window  window   = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);;
    Visual  *visual  = DefaultVisual(display, 0);
    XEvent  event;
    XImage  *ximage;
    unsigned char *image32 = (unsigned char *)malloc(width*height*4);

    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
    if(visual->class != TrueColor) {
        fprintf(stderr, "Cannot handle non true color visual ...\n");
        exit(1);
    }

    ximage = XCreateImage(display, visual, 24, ZPixmap, 0, image32, width, height, 32, 0);

    XSelectInput(display, window, ButtonPressMask | ExposureMask | KeyPressMask);
    XMapWindow(display, window);
    
    setup();

    while(1) {
        XNextEvent(display, &event);
        
        switch ( event.type ) {
            case Expose: {
                //Redraw(ximage);
                draw(ximage);
                XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
                //XFlush(display);
                break;
            }
            case KeyPress: {
                goto exit_loop;
                break;
            }
            default: {
                
            }
        }
        XSync(display, 0);
    }

    exit_loop:
    XCloseDisplay(display);
    return 0;
}
