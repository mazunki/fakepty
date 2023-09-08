
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void printfd(int fd, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *prefix;
    if (fd == 1) {
        prefix = "[out]  ";
    } else if (fd == 2) {
        prefix = "[error]";
    } else {
        prefix = "[other]";
    }

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        printf("%s ", prefix);
		printf(format, line);
        line = strtok(NULL, "\n");
        if (line) {
            prefix = "       ";
        }
    }

    va_end(args);
}

