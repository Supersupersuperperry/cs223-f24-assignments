#include <stdio.h>
#include <string.h>  // For strcmp and strcpy

// Define a struct to store song information
struct song {
    char title[32];
    char artist[32];
    int duration_min;
    int duration_sec;
    float danceability;
};

// Function to print the list of songs
void print_songs(struct song songs[], int num_songs) {
    printf("=====================================\n");
    for (int i = 0; i < num_songs; i++) {
        printf("%d) %-20s artist: %-20s duration: %d:%02d  danceability: %.2f\n", 
               i, songs[i].title, songs[i].artist, 
               songs[i].duration_min, songs[i].duration_sec, songs[i].danceability);
    }
    printf("=====================================\n");
}

int main() {
    // Initialize an array of songs
    struct song songs[3] = {
        {"chapter1", "estatic fear", 4, 11, 0.50},
        {"Glades of Summer", "Sirenia", 5, 36, 0.70},
        {"A view from the Top of the World", "Dream Theater", 19, 34, 0.90}
    };
    
    int song_id;
    char attribute[20];
    int num_songs = 3;  // Number of songs in the list
    
    // Print the list of songs
    print_songs(songs, num_songs);
    
    // Prompt the user to edit a song
    printf("Enter a song id to edit [0,1,2]: ");
    scanf("%d%*c", &song_id);  // Read song id
    
    // Check if the song_id is valid
    if (song_id < 0 || song_id >= num_songs) {
        printf("Invalid song id!\n");
        return 1;
    }
    
    // Ask which attribute to edit
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf("%s", attribute);  // Read the attribute to edit

    // Edit the song based on the attribute
    if (strcmp(attribute, "artist") == 0) {
        printf("Enter a new artist: ");
        scanf(" %[^\n]%*c", songs[song_id].artist);  // Update artist
    } 
    else if (strcmp(attribute, "title") == 0) {
        printf("Enter a new title: ");
        scanf(" %[^\n]%*c", songs[song_id].title);  // Update title
    } 
    else if (strcmp(attribute, "duration") == 0) {
        printf("Enter new duration (minutes): ");
        scanf("%d", &songs[song_id].duration_min);  // Update duration (minutes)
        printf("Enter new duration (seconds): ");
        scanf("%d", &songs[song_id].duration_sec);  // Update duration (seconds)
    } 
    else if (strcmp(attribute, "danceability") == 0) {
        printf("Enter new danceability: ");
        scanf("%f", &songs[song_id].danceability);  // Update danceability
    } 
    else {
        printf("Invalid attribute!\n");
        return 1;
    }
    
    // Print the updated list of songs
    print_songs(songs, num_songs);
    
    return 0;
}
