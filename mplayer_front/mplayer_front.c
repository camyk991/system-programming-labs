#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define FIFO_PATH "/tmp/mplayer_fifo"

void handle_sigint(int sig) {
    // Cleanup logic on Ctrl+C
    printf("\nReceived SIGINT, exiting...\n");
    unlink(FIFO_PATH);
    exit(0);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint; 
    sa.sa_flags = 0;  
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    if (mkfifo(FIFO_PATH, 0666) == -1) {        
        perror("mkfifo");
        exit(1);
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        unlink(FIFO_PATH);
        exit(1);
    }

    printf("Commands: \n");
    printf("p - pause, + - seek forward, - - seek backward, q - quit\n");

    char command;
    int isOn = 1;
    while (isOn) {
        printf("Podaj komende: ");
        command = getchar();
        getchar();

        char mplayer_command[10];

        switch (command) {
            case 'p':
                strcpy(mplayer_command, "pause\n");
                break;
            case '+':
                strcpy(mplayer_command, "seek 5\n");
                break;
            case '-':
                strcpy(mplayer_command, "seek -5\n");
                break;
            case 'q':
                strcpy(mplayer_command, "quit\n");
                isOn = 0;
                break;
            default:
                printf("Unknown command \n");
                continue;        
        }

        if (write(fd, mplayer_command, strlen(mplayer_command)) == -1) {
            perror("write");
        }
    }

    close(fd);
    unlink(FIFO_PATH);
    return 0;
}
