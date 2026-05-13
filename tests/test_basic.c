#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_dummy(void **state) {
    assert_int_equal(1, 1);
}
