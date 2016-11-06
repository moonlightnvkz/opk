#ifndef READER_H
#define READER_H
#include <stdbool.h>

bool read_value(const char *request, const char *fmt, void *value);

char * read_line(const char * request);

#endif // READER_H
