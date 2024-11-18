#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "write_ppm.h"
#include <math.h>


// Custom pthread barrier implementation for macOS
typedef struct {
    int count;            // Number of threads currently waiting
    int target;           // Total number of threads to wait for
    pthread_mutex_t lock; // Mutex for synchronizing access
    pthread_cond_t cond;  // Condition variable for waiting threads
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *barrier, void *attr, int count) {
    barrier->count = 0;
    barrier->target = count;
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->lock);
    barrier->count++;
    if (barrier->count == barrier->target) {
        barrier->count = 0; // Reset for reuse
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->lock);
        return 1; // Indicate that this thread is the "barrier leader"
    } else {
        pthread_cond_wait(&barrier->cond, &barrier->lock);
        pthread_mutex_unlock(&barrier->lock);
        return 0; // Other threads
    }
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
    pthread_mutex_destroy(&barrier->lock);
    pthread_cond_destroy(&barrier->cond);
    return 0;
}

// Shared data structure
typedef struct {
    int size;              // Image size (width and height)
    float xmin, xmax;      // X range of the Buddhabrot
    float ymin, ymax;      // Y range of the Buddhabrot
    int maxIterations;     // Maximum iterations for escape calculation
    int **counts;          // Array to store visitation counts
    pthread_mutex_t mutex; // Mutex for thread-safe count updates
    pthread_barrier_t barrier; // Barrier to synchronize threads
} SharedData;

// Thread-specific data
typedef struct {
    int id;               // Thread ID
    SharedData *sharedData; // Pointer to shared data
} ThreadData;

// Thread function
void *thread_routine(void *arg) {
    ThreadData *threadData = (ThreadData *)arg;
    SharedData *shared = threadData->sharedData;
    int id = threadData->id;
    int size = shared->size;

    // Calculate the image quadrant this thread is responsible for
    int rowsPerThread = size / 2;
    int colsPerThread = size / 2;
    int rowStart = (id / 2) * rowsPerThread;
    int colStart = (id % 2) * colsPerThread;

    // Step 1: Iterate over assigned pixels
    for (int row = rowStart; row < rowStart + rowsPerThread; row++) {
        for (int col = colStart; col < colStart + colsPerThread; col++) {
            float xfrac = (float)row / size;
            float yfrac = (float)col / size;
            float x0 = shared->xmin + xfrac * (shared->xmax - shared->xmin);
            float y0 = shared->ymin + yfrac * (shared->ymax - shared->ymin);

            float x = 0, y = 0;
            int iter = 0;
            while (iter < shared->maxIterations && x * x + y * y < 4) {
                float xtmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtmp;
                iter++;
            }

            // Step 2: Track escape trajectory
            if (iter < shared->maxIterations) {
                x = 0;
                y = 0;
                while (x * x + y * y < 4) {
                    float xtmp = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xtmp;

                    int pixelRow = (int)((y - shared->ymin) / (shared->ymax - shared->ymin) * size);
                    int pixelCol = (int)((x - shared->xmin) / (shared->xmax - shared->xmin) * size);

                    if (pixelRow >= 0 && pixelRow < size && pixelCol >= 0 && pixelCol < size) {
                        pthread_mutex_lock(&shared->mutex);
                        shared->counts[pixelRow][pixelCol]++;
                        pthread_mutex_unlock(&shared->mutex);
                    }
                }
            }
        }
    }

    pthread_barrier_wait(&shared->barrier);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Default parameters
    int size = 480;
    float xmin = -2.0, xmax = 0.47, ymin = -1.12, ymax = 1.12;
    int maxIterations = 1000;
    int numProcesses = 4;

    // Parse command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'l': xmin = atof(optarg); break;
            case 'r': xmax = atof(optarg); break;
            case 't': ymax = atof(optarg); break;
            case 'b': ymin = atof(optarg); break;
            case 'p': numProcesses = atoi(optarg); break;
            default:
                printf("Usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Print configuration
    printf("Generating Buddhabrot with size %dx%d\n", size, size);

    // Initialize shared data
    SharedData sharedData = {
        .size = size,
        .xmin = xmin,
        .xmax = xmax,
        .ymin = ymin,
        .ymax = ymax,
        .maxIterations = maxIterations,
        .counts = malloc(size * sizeof(int*))
    };

    for (int i = 0; i < size; i++) {
        sharedData.counts[i] = calloc(size, sizeof(int));
    }
    pthread_mutex_init(&sharedData.mutex, NULL);
    pthread_barrier_init(&sharedData.barrier, NULL, numProcesses);

    // Create threads
    pthread_t threads[numProcesses];
    ThreadData threadData[numProcesses];
    for (int i = 0; i < numProcesses; i++) {
        threadData[i] = (ThreadData){.id = i, .sharedData = &sharedData};
        pthread_create(&threads[i], NULL, thread_routine, &threadData[i]);
    }

    // Wait for threads to complete
    for (int i = 0; i < numProcesses; i++) {
        pthread_join(threads[i], NULL);
    }

    // Convert counts to colors and write to PPM
    struct ppm_pixel* pixels = malloc(size * size * sizeof(struct ppm_pixel));
    int maxCount = 0;

    // Find the maximum count
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (sharedData.counts[i][j] > maxCount) {
                maxCount = sharedData.counts[i][j];
            }
        }
    }

    // Normalize counts to colors
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int index = i * size + j;
            float value = sharedData.counts[i][j] > 0
                ? log(sharedData.counts[i][j]) / log(maxCount)
                : 0;

            pixels[index].red = (unsigned char)(value * 255);
            pixels[index].green = (unsigned char)(value * 255 * 0.6); // Green tint
            pixels[index].blue = (unsigned char)(value * 255 * 0.3);  // Blue tint
        }
    }

    write_ppm("buddhabrot.ppm", pixels, size, size);

    // Cleanup
    for (int i = 0; i < size; i++) {
        free(sharedData.counts[i]);
    }
    free(sharedData.counts);
    free(pixels);
    pthread_mutex_destroy(&sharedData.mutex);
    pthread_barrier_destroy(&sharedData.barrier);

    return 0;
}
