#pragma once

#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      printf("  [ASSERT FAILED] %s at %s:%d\n", #condition, __FILE__,          \
             __LINE__);                                                        \
      return false;                                                            \
    }                                                                          \
  } while (0)

#define INIT_TEST_MODULE(module)                                               \
  int module##_successful_tests = 0;                                           \
  int module##_total_tests = 0;                                                \
  char *module##_test_module_name = #module

#define RUN_TEST(module, test_func)                                            \
  do {                                                                         \
    module##_total_tests++;                                                    \
    printf("[%s] [%s] ", module##_test_module_name,          \
           #test_func);                                                        \
    if (test_func()) {                                                         \
      module##_successful_tests++;                                             \
      printf("✓ PASSED\n\n");                                                  \
    } else {                                                                   \
      printf("✗ FAILED\n\n");                                                  \
    }                                                                          \
  } while (0)

#define PRINT_TEST_SUMMARY(module)                                             \
  printf("----------------------------------------\n");                        \
  printf("[%s] Summary: %d / %d tests passed.\n", module##_test_module_name,   \
         module##_successful_tests, module##_total_tests);                     \
  printf("----------------------------------------\n\n");                      \
  return module##_total_tests - module##_successful_tests;
