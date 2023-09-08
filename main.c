#include <fcntl.h>
#include <pty.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

#include "printfd.h"

int main(void) {
    int our_fd_stdout, our_fd_stderr, gdb_fd_stdout, gdb_fd_stderr;
    char gdb_path_stdout[1024], gdb_path_stderr[1024];

    if (openpty(&our_fd_stdout, &gdb_fd_stdout, gdb_path_stdout, NULL, NULL) == -1) {
        perror("openpty stdout");
        return 1;
    }
    if (openpty(&our_fd_stderr, &gdb_fd_stderr, gdb_path_stderr, NULL, NULL) == -1) {
        perror("openpty stderr");
        return 1;
    }

    printf("pseudo-terminals created\n");
    printf("set inferior-tty %s\n", gdb_path_stdout);
    printf("2>%s\n", gdb_path_stderr);
    printf("\n");

    fd_set read_fds;
    char buffer[256];
    int n;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(our_fd_stdout, &read_fds);
        FD_SET(our_fd_stderr, &read_fds);

        int max_fd = our_fd_stdout > our_fd_stderr ? our_fd_stdout : our_fd_stderr;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            break;
        }

        if (FD_ISSET(our_fd_stdout, &read_fds)) {
            n = read(our_fd_stdout, buffer, sizeof(buffer) - 1);
            if (n <= 0) {
                perror("read stdout");
                break;
            }
            buffer[n] = '\0';
            printfd(1, "%s", buffer);
        }

        if (FD_ISSET(our_fd_stderr, &read_fds)) {
            n = read(our_fd_stderr, buffer, sizeof(buffer) - 1);
            if (n <= 0) {
                perror("read stderr");
                break;
            }
            buffer[n] = '\0';
            printfd(2, "%s", buffer);
        }
    }

    close(gdb_fd_stdout);
    close(gdb_fd_stderr);
    close(our_fd_stdout);
    close(our_fd_stderr);

    return 0;
}

