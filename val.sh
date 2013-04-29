#! /bin/sh
valgrind --tool=helgrind --log-file=helgrind.txt --dsymutil=yes ./main 2000 50 0 1 ./sampleFiles/sampleAbortTrx.txt ./sampleFiles/sampleShortProcess.txt