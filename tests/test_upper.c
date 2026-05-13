#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "../include/utils.h"   // doit contenir la déclaration de to_upper()

void test_to_upper(void **state) {
    (void) state;

    char buffer[32] = "Bonjour";
    to_upper(buffer);

    assert_string_equal(buffer, "BONJOUR");
}
