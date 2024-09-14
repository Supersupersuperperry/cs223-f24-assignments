#include <stdio.h>
#include <stdlib.h>  // for rand()
#include <time.h>    // for time()

int main() {
    char buff[32];  // Buffer to store the input word
    char symbols[] = {'@', '!', '#', '*'};  // Array of symbols to randomly replace lowercase letters

    // Seed the random number generator
    srand(time(0));
    
    // Prompt the user for input
    printf("Enter a word: ");
    scanf("%s", buff);  // Read the input word
    
    // Loop through each character in the input word
    for (int i = 0; buff[i] != '\0'; i++) {
        // If the character is a lowercase letter, replace it with a random symbol
        if (buff[i] >= 'a' && buff[i] <= 'z') {
            buff[i] = symbols[rand() % 4];  // Randomly select one of the symbols
        }
    }

    // Output the modified word
    printf("OMG! %s\n", buff);
    
    return 0;
}
