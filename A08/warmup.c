#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid_b, pid_c;

    printf("%d] A0\n", getpid());  // Parent process prints A0

    // Create B0 process
    pid_b = fork();
    if (pid_b < 0) {
        perror("Fork failed for B0");
        exit(1);
    } else if (pid_b == 0) {
        // Child process B0
        printf("%d] B0\n", getpid());

        // Create B1 process within B0
        pid_t pid_b1 = fork();
        if (pid_b1 < 0) {
            perror("Fork failed for B1");
            exit(1);
        } else if (pid_b1 == 0) {
            // B1 child process
            printf("%d] B1\n", getpid());
            printf("%d] Bye\n", getpid());
            exit(0);
        }

        // Wait for B1 process to complete before B0 exits
        wait(NULL);
        printf("%d] Bye\n", getpid());
        exit(0);
    }

    // Wait for B0 to complete before proceeding to C0
    wait(NULL);

    // Parent process continues and creates C0 process
    printf("%d] C0\n", getpid());

    pid_c = fork();
    if (pid_c < 0) {
        perror("Fork failed for C1");
        exit(1);
    } else if (pid_c == 0) {
        // Child process C1
        printf("%d] C1\n", getpid());
        printf("%d] Bye\n", getpid());
        exit(0);
    }

    // Wait for C1 to complete before parent exits
    wait(NULL);
    printf("%d] Bye\n", getpid());  // Parent process ends
    return 0;
}
