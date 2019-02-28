make cleanLogs
valgrind --tool=memcheck --leak-check=full --log-file=valgrind_info.log ./test.x
lcov --capture --directory . --output-file VHashTable.info
genhtml VHashTable.info --output-directory covinfo