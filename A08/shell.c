#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <pwd.h>
#include <limits.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void print_prompt() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        struct passwd *pw = getpwuid(geteuid());  // Get current user
        printf(ANSI_COLOR_GREEN "%s:%s$ " ANSI_COLOR_RESET, pw->pw_name, cwd);
    } else {
        perror("getcwd() error");
    }
}

int main() {
    char *input;

    // Main shell loop
    while (1) {
        // Display prompt
        print_prompt();

        // Read user input
        input = readline(NULL);
        
        // Add input to history if not NULL
        if (input && *input) {
            add_history(input);
        }

        // Check for exit command
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Parse the input into command and arguments
        char *args[64];  // Assuming max 64 arguments
        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        // Fork and execute the command
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // In child process
            if (execvp(args[0], args) == -1) {
                perror("Command execution failed");
            }
            exit(EXIT_FAILURE);
        } else {
            // In parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Process exited with status %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Process terminated by signal %d\n", WTERMSIG(status));
            }
        }

        free(input);  // Free memory allocated by readline
    }

    return 0;
}

