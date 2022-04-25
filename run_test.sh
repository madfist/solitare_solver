#!/bin/sh

if [ "$1" = "-d" ]; then
  shift
  cd test/test_solver
  gdb ../../build/test/test_solver/solver_test $@
  cd -
else
  make CTEST_OUTPUT_ON_FAILURE=1 -Cbuild test
fi
