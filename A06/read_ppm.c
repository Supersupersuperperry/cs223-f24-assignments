/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 4, 2024
 * Description：This file contains the implementation of the `read_ppm` function, 
 * which reads a binary PPM (P6) image file and loads the pixel data into 
 * a dynamically allocated array. The function parses the image header 
 * (width, height, max color value) and reads the pixel data.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Function to read a PPM file in binary format using a flat array
struct ppm_pixel* read_ppm(const char* filename, int* width, int* height) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Error: could not open file %s\n", filename);
        return NULL;
    }

    printf("Opened file: %s\n", filename);

    // Read and check magic number (P6)
    char magic[3];
    fgets(magic, sizeof(magic), fp);
    if (magic[0] != 'P' || magic[1] != '6') {
        printf("Error: invalid PPM file format (must be P6)\n");
        fclose(fp);
        return NULL;
    }
    printf("Magic number: %s\n", magic);

    // Skip comments and empty lines
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // Skip lines that are comments (start with '#') or are empty
        if (line[0] == '#') {
            printf("Skipping comment line: %s", line);
            continue;
        }

        if (strlen(line) == 0 || line[0] == '\n') {
            printf("Skipping empty line\n");
            continue;
        }

        // If we reach here, we've found the line with width and height
        break;
    }

    // Now we have a non-comment, non-empty line for width and height
    printf("Line read for width and height: %s\n", line);  // Debugging output
    if (sscanf(line, "%d %d", width, height) != 2) {
        printf("Error: failed to read width and height from line: %s\n", line);
        fclose(fp);
        return NULL;
    }
    printf("Width: %d, Height: %d\n", *width, *height);

    // Read the max color value
    fgets(line, sizeof(line), fp);
    printf("Line read for max color value: %s\n", line);  // Debugging output
    int max_val;
    if (sscanf(line, "%d", &max_val) != 1) {
        printf("Error: failed to read max color value from line: %s\n", line);
        fclose(fp);
        return NULL;
    }
    printf("Max value: %d\n", max_val);

    fgetc(fp);  // Read the single whitespace after max_val

    // Allocate memory for pixel data (flat array)
    struct ppm_pixel* pixels = (struct ppm_pixel*) malloc((*width) * (*height) * sizeof(struct ppm_pixel));
    if (!pixels) {
        printf("Error: unable to allocate memory for %d x %d pixels\n", *width, *height);
        fclose(fp);
        return NULL;
    }

    // Read the pixel data
    fread(pixels, sizeof(struct ppm_pixel), (*width) * (*height), fp);

    fclose(fp);  // Close the file
    return pixels;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}