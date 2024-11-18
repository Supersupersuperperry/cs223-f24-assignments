#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

#define MAX_ITER 1000  // Maximum number of iterations for Mandelbrot calculation

struct ppm_pixel palette[MAX_ITER];  // Color palette for Mandelbrot visualization

// Function to generate a random color palette for Mandelbrot set visualization
void generate_palette() {
  for (int i = 0; i < MAX_ITER; i++) {
    palette[i].red = rand() % 256;
    palette[i].green = rand() % 256;
    palette[i].blue = rand() % 256;
  }
}

// Function to compute the Mandelbrot set and store the result in the pixel array
void compute_mandelbrot(int size, float xmin, float xmax, float ymin, float ymax, struct ppm_pixel *pixels) {
  for (int row = 0; row < size; row++) {
    for (int col = 0; col < size; col++) {
      // Map pixel coordinate to complex plane
      float xfrac = (float)row / (float)size;
      float yfrac = (float)col / (float)size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;
      
      // Iteratively check if point escapes the Mandelbrot set bounds
      while (iter < MAX_ITER && x * x + y * y < 4) {
        float xtmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iter++;
      }

      // Assign color based on iteration count (escape speed)
      if (iter < MAX_ITER) {
        pixels[row * size + col] = palette[iter];
      } else {
        pixels[row * size + col].red = 0;
        pixels[row * size + col].green = 0;
        pixels[row * size + col].blue = 0;
      }
    }
  }
}

// Function to save the generated image to a PPM file
void save_image(const char *filename, struct ppm_pixel *pixels, int size) {
  write_ppm(filename, pixels, size, size);
  printf("Writing file: %s\n", filename);
}

int main(int argc, char* argv[]) {
  int size = 480;         // Default image size
  float xmin = -2.0;      // Default minimum x value
  float xmax = 0.47;      // Default maximum x value
  float ymin = -1.12;     // Default minimum y value
  float ymax = 1.12;      // Default maximum y value

  // Parse command-line arguments
  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': 
        printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]);
        return 1;
    }
  }

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // Generate the color palette
  srand(time(0));
  generate_palette();

  // Allocate memory for pixel array
  struct ppm_pixel *pixels = (struct ppm_pixel*)malloc(size * size * sizeof(struct ppm_pixel));
  if (pixels == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }

  // Record start time
  struct timeval start, end;
  gettimeofday(&start, NULL);

  // Compute Mandelbrot set and store in pixels array
  compute_mandelbrot(size, xmin, xmax, ymin, ymax, pixels);

  // Record end time and calculate elapsed time
  gettimeofday(&end, NULL);
  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);

  // Create filename with timestamp and save the image
  char filename[64];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  save_image(filename, pixels, size);

  // Free allocated memory
  free(pixels);

  return 0;
}
