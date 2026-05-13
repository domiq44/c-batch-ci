#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>

#include "../include/io.h"   // doit contenir read_file()

void test_read_file(void **state) {
    (void) state;

    const char *filename = "test_input.txt";
    const char *content = "hello world";

    FILE *f = fopen(filename, "w");
    assert_non_null(f);
    fputs(content, f);
    fclose(f);

    char buffer[64] = {0};
    int ret = read_file(filename, buffer, sizeof(buffer));

    assert_int_equal(ret, 0);
    assert_string_equal(buffer, content);

    remove(filename);
}
