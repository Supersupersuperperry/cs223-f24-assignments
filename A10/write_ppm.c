#include <stdio.h>
#include <stdlib.h>
#include "write_ppm.h"

// Function to write pixel data to a binary PPM file
void write_ppm(const char* filename, struct ppm_pixel* pxs, int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: could not open file %s\n", filename);
        return;
    }

    // Write the header
    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    fwrite(pxs, sizeof(struct ppm_pixel), width * height, fp);

    fclose(fp);  // Close the file
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}