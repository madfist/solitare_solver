#!/bin/sh

cd test/test_solver
if [ "$1" = "-d" ]; then
  gdb solver_test
else
  ./solver_test
fi
cd -
