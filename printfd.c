
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void printfd(int fd, const char *format, ...) {
    char *prefix;
    if (fd == 1) {
        prefix = "  [" ANSI_COLOR_GREEN "out" ANSI_COLOR_RESET "]";
    } else if (fd == 2) {
        prefix = "[" ANSI_COLOR_RED "error" ANSI_COLOR_RESET "]";
    } else {
        prefix = "[other]";
    }

    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        printf("%s ", prefix);

		va_start(args, format);
		printf("%s\n", line);
		va_end(args);

        line = strtok(NULL, "\n");
    if (fd == 1) {
        prefix = "   " ANSI_COLOR_GREEN "..." ANSI_COLOR_RESET " ";
    } else if (fd == 2) {
        prefix = " " ANSI_COLOR_RED "....." ANSI_COLOR_RESET " ";
    } else {
        prefix = "[other]";
    }

    }
}

