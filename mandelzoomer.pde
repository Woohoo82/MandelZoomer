double zoom  = 0.999;
float  angle = 0.0;
double w = 5.0;
double h;
int maxiterations = 200;
color   cl_inside = #ffffff;
color[]   palette = new color[maxiterations];

void setup() {
  size(640, 480);
  h = (w * height) / width;
  colorMode(HSB, 1);
  
  for (int i = 0; i < maxiterations; i++) {
    float inten = sqrt(float(i) / maxiterations);
    palette[i] = color(inten);
  }
}

void draw() {
  background(255);
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
  
  loadPixels();
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
  updatePixels();
  println(frameRate);
}
