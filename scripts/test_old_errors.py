"""

If any of the automatically generated tests fail, the commands to re-run the test is logged in "error_log.txt".

This script runs all the test cases that have previously failed, acting as regression test for the random tests.

"""

import os

if __name__ == "__main__":
  err_log = "error_log.txt"
  ntests = 0
  nok = 0

  print("\nRunning test cases from error log (cases that failed during development).\n")

  if os.path.exists(err_log):
    tests = open(err_log).readlines()
    tetst = [t for t in tests if t]
    ntests = len(tests)
    for t in tests:
      err_code = os.system(t)
      if err_code == 0:
        nok += 1
  print("%d/%d tests passed.\n" % (nok, ntests))

  
