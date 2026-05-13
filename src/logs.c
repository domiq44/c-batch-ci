#include "logs.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static FILE* open_log_file() {
    return fopen("/app/logs/batch.log", "a");
}

static void write_log(const char* level, const char* fmt, va_list args) {
    FILE* f = open_log_file();
    if (!f)
        return;

    time_t now = time(NULL);
    const struct tm* t = localtime(&now);

    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] %s ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour,
            t->tm_min, t->tm_sec, level);
    vfprintf(f, fmt, args);
    fprintf(f, "\n");

    fclose(f);
}

void log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    printf("[INFO] ");
    vprintf(fmt, args);
    printf("\n");

    va_end(args);

    va_start(args, fmt);
    write_log("INFO", fmt, args);
    va_end(args);
}

void log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);

    va_start(args, fmt);
    write_log("ERROR", fmt, args);
    va_end(args);
}

static int is_debug_enabled() {
    const char* lvl = getenv("LOG_LEVEL");
    return lvl && strcmp(lvl, "DEBUG") == 0;
}

void log_debug(const char* fmt, ...) {
    if (!is_debug_enabled())
        return;

    va_list args;
    va_start(args, fmt);

    printf("[DEBUG] ");
    vprintf(fmt, args);
    printf("\n");

    va_end(args);

    va_start(args, fmt);
    write_log("DEBUG", fmt, args);
    va_end(args);
}

void log_message(const char* file, const char* level, const char* msg) {
    FILE* f = fopen(file, "a");
    if (!f)
        return;

    fprintf(f, "[%s] %s\n", level, msg);
    fclose(f);
}
