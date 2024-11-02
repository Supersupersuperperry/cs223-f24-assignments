/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 4, 2024
 * Description：This file tests the functionality of the `read_ppm` function by 
 * reading a PPM file and printing the pixel data for the first 10 rows 
 * of the image. The program ensures that the PPM file is correctly 
 * parsed and the pixel data is accurately loaded.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
    int w, h;
    struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
    if (!pixels) {
        fprintf(stderr, "Failed to read PPM file\n");
        return 1;
    }

    // Print pixel data (first few rows as example)
    for (int i = 0; i < h && i < 10; i++) {  // Limit to 10 rows
        for (int j = 0; j < w; j++) {
            struct ppm_pixel p = pixels[i * w + j];
            printf("(%d,%d,%d) ", p.red, p.green, p.blue);
        }
        printf("\n");
    }

    free(pixels);
    return 0;
}

