#include "parser_test.h"

#include <stdio.h>

int main(void) {
    int parser_failed_tests = test_parser();

    if (parser_failed_tests > 0) {
        printf("Parser failed tests: %d\n", parser_failed_tests);
    } else {
      printf("All parser tests passed\n");
    }
}
