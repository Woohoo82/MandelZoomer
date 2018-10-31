 /*
  * gcc main.c -o mander -lX11
  */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

int width  = 512;
int height = 512;

void Redraw(XImage *image) {
    unsigned int *pixels = (unsigned int *)image->data;
    int x, y;
    
    for (x=100; x<300; x++) {
      for (y=100; y<400; y++) {
        pixels[width*y+x] = 0xffffff;
      }
    }
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

    while(1) {
        XNextEvent(display, &event);
 
        if (event.type == Expose) {
            Redraw(ximage);
            XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
        }

        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);
    return 0;
}
