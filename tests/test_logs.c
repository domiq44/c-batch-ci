#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>

#include "../include/logs.h"

void test_log_message(void **state) {
    (void) state;

    const char *test_file = "test_log_output.txt";
    const char *msg = "Hello log";

    log_message(test_file, "INFO", msg);

    FILE *f = fopen(test_file, "r");
    assert_non_null(f);

    char buffer[256];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);

    assert_true(strstr(buffer, "INFO") != NULL);
    assert_true(strstr(buffer, msg) != NULL);

    remove(test_file);
}
