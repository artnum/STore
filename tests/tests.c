#include "tests.h"

int main (int argc, char ** argv) {
  SRunner * sr = NULL;

  sr = srunner_create (mem_test_suite());
  srunner_add_suite(sr, ustr_test_suite());
  
  srunner_run_all(sr, CK_NORMAL);
  
  return 0;
}

