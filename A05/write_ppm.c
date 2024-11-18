/*----------------------------------------------
 * Author: Yupei Sun    
 * Date: Oct 4, 2024
 * Description: This function writes pixel data to a binary PPM (P6) file. It 
 * writes the image header (width, height, max color value) and 
 * then writes the RGB pixel data to the file.
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: could not open file %s for writing\n", filename);
        return;
    }

    // Write the header: magic number, width, height, and max color value
    fprintf(fp, "P6\n%d %d\n255\n", w, h);

    // Write pixel data
    fwrite(pixels, sizeof(struct ppm_pixel), w * h, fp);

    // Close the file
    fclose(fp);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
