/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Oct 11
 * Description: Convert 64-bit integer to 8x8 1bpp image
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // Iterate over each bit from the most significant to the least significant
  for (int i = 63; i >= 0; i--) {
    // Use a bitmask to check if the current bit is 1 or 0
    unsigned long mask = 0x1ul << i;
    if (img & mask) {
      printf("@"); // 1 represents a black pixel
    } else {
      printf(" "); // 0 represents a white pixel
    }

    // Print a new line every 8 bits to form the 8x8 grid
    if (i % 8 == 0) {
      printf("\n");
    }
  }

  return 0;
}
