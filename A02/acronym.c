#include <stdio.h>
#include <ctype.h>  // for toupper
#include <string.h> // for strlen

int main() {
    char phrase[1024];  // Buffer to store the input phrase from the user
    char acronym[1024]; // Buffer to store the generated acronym
    int j = 0;          // Index for the acronym array
    
    // Prompt the user for input
    printf("Enter a phrase: ");
    scanf("%[^\n]%*c", phrase);  // Read the entire line of input until newline
    
    // Loop through the input phrase
    for (int i = 0; i < strlen(phrase); i++) {
        // Check for uppercase letters or the first letter of a word (after a space)
        if (isupper(phrase[i]) || (i == 0 || phrase[i - 1] == ' ')) {
            // If the letter is lowercase, convert to uppercase
            if (isalpha(phrase[i])) {
                acronym[j] = toupper(phrase[i]);
                j++;  // Move to the next position in the acronym array
            }
        }
    }

    // Terminate the acronym with a null character
    acronym[j] = '\0';

    // Output the generated acronym
    printf("Your acronym is %s\n", acronym);

    return 0;
}
