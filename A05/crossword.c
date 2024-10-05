/*----------------------------------------------
 * Author: Your Name
 * Date: 2024-10-04
 * Description: 
 * This program takes two words as input from the command line and 
 * generates a crossword puzzle by identifying a common letter 
 * between them. One word is placed vertically and the other horizontally.
 * If no common letter is found, the program will print an error message.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  // Check if the correct number of arguments is provided
  if (argc < 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    return 1;
  }

  // Get the words from command line arguments
  char* word1 = argv[1];
  char* word2 = argv[2];

  // Find the common letter between the two words
  int len1 = strlen(word1);
  int len2 = strlen(word2);
  int common_index1 = -1, common_index2 = -1;

  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      if (word1[i] == word2[j]) {
        common_index1 = i;
        common_index2 = j;
        break;
      }
    }
    if (common_index1 != -1) break;
  }

  // If no common letter is found, print a message and exit
  if (common_index1 == -1) {
    printf("No common letter!\n");
    return 0;
  }

  // Dynamically allocate memory for the crossword grid
  int rows = len2;
  int cols = len1;
  char** crossword = (char**) malloc(rows * sizeof(char*));
  for (int i = 0; i < rows; i++) {
    crossword[i] = (char*) malloc(cols * sizeof(char));
    for (int j = 0; j < cols; j++) {
      crossword[i][j] = '.';  // Fill the grid with dots
    }
  }

  // Place word1 horizontally and word2 vertically in the crossword
  for (int i = 0; i < len1; i++) {
    crossword[common_index2][i] = word1[i];
  }
  for (int j = 0; j < len2; j++) {
    crossword[j][common_index1] = word2[j];
  }

  // Print the crossword
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%c ", crossword[i][j]);
    }
    printf("\n");
  }

  // Free the allocated memory
  for (int i = 0; i < rows; i++) {
    free(crossword[i]);
  }
  free(crossword);

  return 0;
}
