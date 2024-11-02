/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 4, 2024
 * Description: 
 * This program reads a PPM file, writes the pixel data to another PPM file,
 * reloads the newly written file, and prints its pixel contents.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
    int w, h;

    // Read the original PPM file
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
    if (!pixels) {
        fprintf(stderr, "Failed to read PPM file\n");
        return 1;
    }

    // Write the pixel data to a new PPM file
    write_ppm("test.ppm", pixels, w, h);

    // Free the original pixels
    free(pixels);

    // Reload the newly written PPM file
    pixels = read_ppm("test.ppm", &w, &h);
    if (!pixels) {
        fprintf(stderr, "Failed to reload test.ppm\n");
        return 1;
    }

    // Print the pixel values of the reloaded PPM file (limit to the first 10 rows)
    for (int i = 0; i < h && i < 10; i++) {  // Limit to 10 rows for large images
        for (int j = 0; j < w; j++) {
            struct ppm_pixel p = pixels[i * w + j];  // Access flat array
            printf("(%d,%d,%d) ", p.red, p.green, p.blue);
        }
        printf("\n");
    }

    free(pixels);
    return 0;
}
