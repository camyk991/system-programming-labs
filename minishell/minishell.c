#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_COMMANDS 10
#define MAX_ARGS 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command1> . <command2> . ...\n", argv[0]);
        return 1;
    }

    int pipefds[2 * MAX_COMMANDS];
    int command_count = 0;
    char *commands[MAX_COMMANDS][MAX_ARGS];
    int arg_index = 0;

    // argc contains number of arguments
    // argv contains vector
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ".") == 0) {
            commands[command_count][arg_index] = NULL;
            command_count++;
            arg_index = 0;
        } else {
            commands[command_count][arg_index++] = argv[i];
	    printf("%s \n", argv[i]);
        }
    }
    commands[command_count][arg_index] = NULL;
    command_count++;
	
    // Create pipes
    for (int i = 0; i < command_count - 1; i++) {
        if (pipe( &pipefds[i * 2] ) < 0) {
            perror("pipe failed");
            return 1;
        }
    }

    for (int i = 0; i < command_count; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }
        if (pid == 0) {
            // If not the first command, get input from the previous pipe
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }
            // If not the last command, output to the next pipe
            if (i < command_count - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }

            // Close all pipes in the child process
            for (int j = 0; j < 2 * (command_count - 1); j++) {
                close(pipefds[j]);
            }

            // Execute the command
            execvp(commands[i][0], commands[i]);
            perror("execvp failed");
            exit(1);
        }
    }

    // Close all pipes in the parent process
    for (int i = 0; i < 2 * (command_count - 1); i++) {
        close(pipefds[i]);
    }

    // Wait for all child processes
    for (int i = 0; i < command_count; i++) {
        wait(NULL);
    }

    return 0;
}

