/*----------------------------------------------
 * Author: Yupei Sun
 * Date: Sept 27
 * Description: A program to load songs from a CSV file, store them in a linked list, 
 * and allow the user to access the most danceable song.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a song
typedef struct Song {
    char title[100];
    char artist[100];
    int duration;
    float danceability;
    float energy;
    float tempo;
    float valence;
    struct Song* next;
} Song;

// Function to load songs from CSV file into a linked list
Song* load_songs(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    Song* head = NULL;
    Song* tail = NULL;
    char line[256];
    fgets(line, sizeof(line), file);  // Skip the header

    while (fgets(line, sizeof(line), file)) {
        Song* new_song = (Song*)malloc(sizeof(Song));
        sscanf(line, "%[^,],%[^,],%d,%f,%f,%f,%f", new_song->title, new_song->artist, 
            &new_song->duration, &new_song->danceability, &new_song->energy, &new_song->tempo, &new_song->valence);
        new_song->next = NULL;

        if (head == NULL) {
            head = new_song;
            tail = new_song;
        } else {
            tail->next = new_song;
            tail = new_song;
        }
    }

    fclose(file);
    return head;
}
// Helper function: Print the entire list of songs
void print_songs_list(Song* head) {
    Song* temp = head;
    int index = 0;
    while (temp != NULL) {
        printf("%d) %s\t%s\t(%.2fs) D: %.3f E: %.3f T: %.3f V: %.3f\n", index, temp->title, temp->artist,
               temp->duration / 1000.0, temp->danceability, temp->energy, temp->tempo, temp->valence);
        temp = temp->next;
        index++;
    }
}

// Helper function: Insert a song at the end of the list
void insert_song(Song** head, Song* new_song) {
    if (*head == NULL) {
        *head = new_song;
    } else {
        Song* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_song;
    }
}

// Function to print a song's information
void print_song(Song* song, int index) {
    printf("%d) %s\t%s\t(%.2fs) D: %.3f E: %.3f T: %.3f V: %.3f\n", index, song->title, song->artist,
           song->duration / 1000.0, song->danceability, song->energy, song->tempo, song->valence);
}

// Function to find and remove the most danceable song
Song* find_and_remove_most_danceable(Song** head) {
    if (*head == NULL) {
        return NULL;
    }

    Song* most_danceable = *head;
    Song* prev = NULL;
    Song* current = *head;
    Song* prev_most = NULL;

    while (current != NULL) {
        if (current->danceability > most_danceable->danceability) {
            most_danceable = current;
            prev_most = prev;
        }
        prev = current;
        current = current->next;
    }

    // Remove the most danceable song from the list
    if (prev_most == NULL) {
        *head = (*head)->next;
    } else {
        prev_most->next = most_danceable->next;
    }

    return most_danceable;
}

// Function to free the entire linked list
void free_songs(Song* head) {
    Song* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Song* song_list = load_songs("songlist.csv");
    if (song_list == NULL) {
        return 1;
    }

    int song_count = 0;
    Song* temp = song_list;
    while (temp != NULL) {
        print_song(temp, song_count);
        song_count++;
        temp = temp->next;
    }

    printf("\nDataset contains %d songs\n", song_count);
    char choice;
    
    while (song_list != NULL) {
        printf("=======================\nPress 'd' to show the most danceable song (any other key to quit): ");
        scanf(" %c", &choice);
        
        if (choice == 'd') {
            Song* most_danceable = find_and_remove_most_danceable(&song_list);
            if (most_danceable != NULL) {
                printf("\n---------------------------------------- Most danceable ------------------------------------\n");
                print_song(most_danceable, 0); 
                printf("--------------------------------------------------------------------------------------------\n");
                free(most_danceable);
            }
        } else {
            break;
        }

        // Print remaining songs
        int index = 0;
        temp = song_list;
        while (temp != NULL) {
            print_song(temp, index);
            index++;
            temp = temp->next;
        }

        printf("\nDataset contains %d songs\n", index);
    }

    free_songs(song_list);
    return 0;
}
