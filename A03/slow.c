/*----------------------------------------------
 * Author: Yupei Sun
 * Date: 2024-09-20
 * Description: It will implement the dynamic memory allocation
 *              and handle song data in C using malloc and free.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char buff[32];
    int pause_len;
    
    // Ask user for the pause length
    printf("Pause length: ");
    scanf("%d", &pause_len);
    
    // Ask user for the text input
    printf("Text: ");
    scanf("%s", buff);  // Read input text

    // Calculate the required space for the output string
    int new_len = strlen(buff) * (pause_len + 1);
    char *ent_phrase = (char *)malloc(new_len + 1);  // Dynamically allocate space

    // Convert the phrase to ent-speech
    int idx = 0;
    for (int i = 0; i < strlen(buff); i++) {
        ent_phrase[idx++] = buff[i];
        // Add dots after every character, including punctuation, except for the very last character
        if (i != strlen(buff) - 1) {
            for (int j = 0; j < pause_len; j++) {
                ent_phrase[idx++] = '.';
            }
        }
    }
    ent_phrase[idx] = '\0';  // Add null terminator at the end of the string

    // Output the ent-speech version of the text
    printf("%s\n", ent_phrase);
    
    // Free the dynamically allocated memory
    free(ent_phrase);

    return 0;
}
