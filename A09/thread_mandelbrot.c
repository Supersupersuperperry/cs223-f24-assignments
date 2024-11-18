#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h> // Include for timeval structure and gettimeofday function
#include "read_ppm.h"
#include "write_ppm.h"

#define MAX_ITER 1000

struct ppm_pixel palette[MAX_ITER];  // Color palette for Mandelbrot visualization

// Structure to pass data to each thread
typedef struct {
  int start_row;
  int end_row;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  struct ppm_pixel *pixels;
} ThreadData;

// Function to generate a random color palette
void generate_palette() {
  for (int i = 0; i < MAX_ITER; i++) {
    palette[i].red = rand() % 256;
    palette[i].green = rand() % 256;
    palette[i].blue = rand() % 256;
  }
}

// Thread function to compute Mandelbrot for a specific image region
void *mandelbrot_thread(void *arg) {
  ThreadData *data = (ThreadData*)arg;
  int size = data->size;

  for (int row = data->start_row; row < data->end_row; row++) {
    for (int col = 0; col < size; col++) {
      // Map pixel coordinate to complex plane
      float xfrac = (float)row / (float)size;
      float yfrac = (float)col / (float)size;
      float x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (data->ymax - data->ymin);

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
        data->pixels[row * size + col] = palette[iter];
      } else {
        data->pixels[row * size + col].red = 0;
        data->pixels[row * size + col].green = 0;
        data->pixels[row * size + col].blue = 0;
      }
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case 'p': numProcesses = atoi(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); return 1;
    }
  }

  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // Generate color palette
  srand(time(0));
  generate_palette();

  // Allocate memory for pixel array
  struct ppm_pixel *pixels = (struct ppm_pixel*)malloc(size * size * sizeof(struct ppm_pixel));
  if (pixels == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    return 1;
  }

  // Variables to store start and end times for performance measurement
  struct timeval start, end;
  gettimeofday(&start, NULL); // Record start time

  // Create and start threads to compute the Mandelbrot set
  pthread_t threads[numProcesses];
  ThreadData thread_data[numProcesses];
  int rows_per_thread = size / numProcesses;

  for (int i = 0; i < numProcesses; i++) {
    thread_data[i].start_row = i * rows_per_thread;
    thread_data[i].end_row = (i == numProcesses - 1) ? size : (i + 1) * rows_per_thread;
    thread_data[i].size = size;
    thread_data[i].xmin = xmin;
    thread_data[i].xmax = xmax;
    thread_data[i].ymin = ymin;
    thread_data[i].ymax = ymax;
    thread_data[i].pixels = pixels;

    pthread_create(&threads[i], NULL, mandelbrot_thread, &thread_data[i]);
  }

  // Wait for all threads to complete
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&end, NULL); // Record end time after threads complete

  // Calculate elapsed time in seconds
  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, elapsed);

  // Save the image file
  char filename[64];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, pixels, size, size);
  printf("Writing file: %s\n", filename);

  // Free allocated memory
  free(pixels);

  return 0;
}
