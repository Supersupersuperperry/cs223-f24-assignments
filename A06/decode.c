/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 4, 2024
 * Description: This file contains the implementation of decoding a hidden message
 * from a PPM image by reading the least significant bits (LSB) of the 
 * image's RGB values.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }

  int width, height;
  
  // Use read_ppm to get pixel data and image dimensions
  struct ppm_pixel* img = read_ppm(argv[1], &width, &height);
  if (img == NULL) {
    printf("Error: Could not open file %s\n", argv[1]);
    return 1;
  }

  printf("Reading %s with width %d and height %d\n", argv[1], width, height);

  // Calculate the maximum number of characters
  int max_chars = (width * height * 3) / 8;
  printf("Max number of characters in the image: %d\n", max_chars);

  // Variables for decoding the message
  unsigned char character = 0;
  int bit_count = 0;

  // Traverse through each pixel to extract the LSB from each RGB value
  for (int i = 0; i < width * height; i++) {
    struct ppm_pixel pixel = img[i];

    // Extract LSB from Red, Green, and Blue channels
    unsigned char bits[3] = {
      pixel.red & 0x1,  // LSB of Red
      pixel.green & 0x1,  // LSB of Green
      pixel.blue & 0x1   // LSB of Blue
    };

    // Combine LSBs to form characters
    for (int j = 0; j < 3; j++) {
      character = (character << 1) | bits[j];
      bit_count++;

      // When we get 8 bits, convert to a character
      if (bit_count == 8) {
        // Debugging output: print each decoded character in hex and ASCII
        printf("Decoded character: %c (0x%02X)\n", character, character);

        if (character == '\0') { // If null terminator is found, stop decoding
          printf("\n");
          free(img);
          return 0;
        }
        printf("%c", character);  // Print the decoded character
        character = 0;  // Reset for the next character
        bit_count = 0;
      }
    }
  }

  printf("\n");

  // Free the allocated memory for the image
  free(img);

  return 0;
}
