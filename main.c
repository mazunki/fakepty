#include <fcntl.h>
#include <pty.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <signal.h>

#include "printfd.h"

int our_fd_stdout, our_fd_stderr, gdb_fd_stdout, gdb_fd_stderr;
char gdb_path_stdout[1024], gdb_path_stderr[1024];
const char *known_stdout_path = "/tmp/gdb_stdout";
const char *known_stderr_path = "/tmp/gdb_stderr";

void graceful_exit(int sig_num) {
    close(gdb_fd_stdout);
    close(gdb_fd_stderr);
    close(our_fd_stdout);
    close(our_fd_stderr);
    unlink(known_stdout_path);
    unlink(known_stderr_path);
	exit(0);
}

int main(void) {
    if (openpty(&our_fd_stdout, &gdb_fd_stdout, gdb_path_stdout, NULL, NULL) == -1) {
        perror("openpty stdout");
        return 1;
    }
    if (openpty(&our_fd_stderr, &gdb_fd_stderr, gdb_path_stderr, NULL, NULL) == -1) {
        perror("openpty stderr");
        return 1;
    }

    symlink(gdb_path_stdout, known_stdout_path);
    symlink(gdb_path_stderr, known_stderr_path);

    signal(SIGINT, graceful_exit);

    printf("pseudo-terminals created\n");
    printf("set inferior-tty %s\n", known_stdout_path);
    printf("2>%s\n", known_stderr_path);
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
            printfd(1, "%s\n", buffer);
        }

        if (FD_ISSET(our_fd_stderr, &read_fds)) {
            n = read(our_fd_stderr, buffer, sizeof(buffer) - 1);
            if (n <= 0) {
                perror("read stderr");
                break;
            }
            buffer[n] = '\0';
            printfd(2, "%s\n", buffer);
        }
    }

	graceful_exit(0);

    return 0;
}

