#include <stdlib.h>
#include <check.h>
#include "../src/micetrap.h"

void
setup (void)
{

}

void
teardown (void)
{

}

START_TEST (test_micetrap_create)
{
  fail_unless (5 == 5,
	       "5 does not equal 5");
  fail_unless (strcmp ("USD", "USD") == 0,
	       "USD does not equal USD");
}
END_TEST

START_TEST (test_micetrap_create_neg)
{
  fail_unless (NULL == NULL,
	       "NULL does not equal NULL");
}
END_TEST

START_TEST (test_micetrap_create_zero)
{
  fail_unless (0 == 0, 
	       "Zero equals 0");
}
END_TEST

Suite *
micetrap_suite (void)
{
  Suite *s = suite_create ("micetrap");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_micetrap_create);
  suite_add_tcase (s, tc_core);

  /* Limits test case */
  TCase *tc_limits = tcase_create ("Limits");
  tcase_add_test (tc_limits, test_micetrap_create_neg);
  tcase_add_test (tc_limits, test_micetrap_create_zero);
  suite_add_tcase (s, tc_limits);

  return s;
}

int
main (void)
{
  int number_failed;
  Suite *s = micetrap_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

