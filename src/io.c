#include <stddef.h>
#include <stdio.h>

int read_file(const char* path, char* buffer, size_t size) {
    FILE* f = fopen(path, "r");
    if (!f)
        return -1;

    if (!fgets(buffer, size, f)) {
        fclose(f);
        return -1;
    }

    fclose(f);
    return 0;
}
