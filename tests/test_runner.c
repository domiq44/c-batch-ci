#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Déclarations des fonctions de test
extern void test_dummy(void **state);
extern void test_log_message(void **state);
extern void test_to_upper(void **state);
extern void test_read_file(void **state);

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dummy),
        cmocka_unit_test(test_log_message),
        cmocka_unit_test(test_to_upper),
        cmocka_unit_test(test_read_file),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
