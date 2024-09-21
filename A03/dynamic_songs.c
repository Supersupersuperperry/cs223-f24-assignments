/*----------------------------------------------
 * Author: Yupei Sun
 * Date: 2024-09-20
 * Description: This program reads a CSV file containing song data,
 *              dynamically stores the data in memory, and prints it.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char title[128];
    char artist[128];
    int duration;   // Duration in milliseconds
    float danceability;
    float energy;
    float tempo;
    float valence;
} Song;

int main() {
    FILE *file = fopen("songlist.csv", "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    char buffer[256];
    int num_songs;

    // Read the first line to get the number of songs
    fgets(buffer, sizeof(buffer), file);
    num_songs = atoi(strtok(buffer, ","));  // Get the number of songs from the first token

    // Dynamically allocate memory for the array of songs
    Song *songs = (Song *)malloc(num_songs * sizeof(Song));
    if (!songs) {
        printf("Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    // Skip the header line
    fgets(buffer, sizeof(buffer), file);

    // Read and store each song's information
    for (int i = 0; i < num_songs; i++) {
        fgets(buffer, sizeof(buffer), file);
        strcpy(songs[i].title, strtok(buffer, ","));
        strcpy(songs[i].artist, strtok(NULL, ","));
        songs[i].duration = atoi(strtok(NULL, ","));
        songs[i].danceability = atof(strtok(NULL, ","));
        songs[i].energy = atof(strtok(NULL, ","));
        songs[i].tempo = atof(strtok(NULL, ","));
        songs[i].valence = atof(strtok(NULL, ","));
    }

    // Print out the song list
    printf("Welcome to Dynamic Donna's Danceability Directory.\n");
    for (int i = 0; i < num_songs; i++) {
        printf("%d) %s by %s\n   Duration: %d ms, Danceability: %.3f, Energy: %.3f, Tempo: %.3f, Valence: %.3f\n",
               i, songs[i].title, songs[i].artist, songs[i].duration, songs[i].danceability, songs[i].energy,
               songs[i].tempo, songs[i].valence);
    }

    // Free the dynamically allocated memory
    free(songs);
    fclose(file);

    return 0;
}
