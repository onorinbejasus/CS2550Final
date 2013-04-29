#! /bin/sh
valgrind --tool=helgrind --log-file=helgrind.txt --dsymutil=yes ./main 2000 50 0 0 ./demoSc/ds1/aTrx.txt