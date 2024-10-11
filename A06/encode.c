/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 11, 2024
 * Description: This file contains the implementation of encoding a message
 * into a PPM image by modifying the least significant bits (LSB) of the 
 * image's RGB values.
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
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

  int max_chars = (width * height * 3) / 8; // Calculate the maximum number of characters
  printf("Max number of characters in the image: %d\n", max_chars);

  // Ask the user for a message to encode
  char message[256];  // You can adjust this size
  printf("Enter a phrase: ");
  fgets(message, sizeof(message), stdin);

  // Check if the message is too long to fit in the image
  if (strlen(message) > max_chars) {
    printf("Error: Message is too long to encode in the image.\n");
    free(img); // free allocated memory
    return 1;
  }

  // Start encoding the message into the image's LSB
  int bit_count = 0;
  int message_index = 0;
  while (message[message_index] != '\0') {
    unsigned char character = message[message_index];

    // Encode each bit of the current character into the image's pixels
    for (int bit = 7; bit >= 0; bit--) {
      unsigned char bit_value = (character >> bit) & 0x1;  // Get the current bit (from MSB to LSB)
      int pixel_index = bit_count / 3;
      if (pixel_index >= width * height) {
        printf("Error: Not enough space in the image to encode the entire message.\n");
        free(img); // free allocated memory
        return 1;
      }

      // Modify the LSB of the pixel's red, green, or blue channel
      switch (bit_count % 3) {
        case 0:  // Red channel
          img[pixel_index].red = (img[pixel_index].red & 0xFE) | bit_value;
          break;
        case 1:  // Green channel
          img[pixel_index].green = (img[pixel_index].green & 0xFE) | bit_value;
          break;
        case 2:  // Blue channel
          img[pixel_index].blue = (img[pixel_index].blue & 0xFE) | bit_value;
          break;
      }
      bit_count++;
    }

    message_index++;
  }

  // Write the modified image to a new file
  char output_filename[256];
  snprintf(output_filename, sizeof(output_filename), "%s-encoded.ppm", argv[1]);
  write_ppm(output_filename, img, width, height); // Pass width and height as parameters

  printf("Writing file %s\n", output_filename);

  // Free the allocated memory for the image
  free(img);

  return 0;
}
