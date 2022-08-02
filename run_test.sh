#!/bin/sh

if [ "$1" = "-d" ]; then
    shift
    make -Cbuild solver_test
    gdb --cd test/test_solver ../../build/test/test_solver/solver_test $@
elif [ "$1" = "-t" ]; then
    shift
    make -Cbuild solver_test
    cd test/test_solver
    ../../build/test/test_solver/solver_test $@ --trace
    cd -
else
    make CTEST_OUTPUT_ON_FAILURE=1 -Cbuild solver_test test
fi
