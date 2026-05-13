#ifndef LOGS_H
#define LOGS_H

void log_info(const char* fmt, ...);
void log_error(const char* fmt, ...);
void log_debug(const char* fmt, ...);
void log_message(const char* file, const char* level, const char* msg);

#endif
