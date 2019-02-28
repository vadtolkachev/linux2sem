CPP=g++
WCPPFLAGS=-Wall -Wextra
COVFLAGS=-fprofile-arcs -ftest-coverage
CPPFLAGS=$(COVFLAGS) $(WCPPFLAGS) -O3 -g -MD -iquote VList -iquote VHashTable -iquote .
OBJFILES=test.o VList/VListNode.o VList/VList.o VHashTable/VHashTable.o

all: test.x

test.x: $(OBJFILES)
	$(CPP) $(CPPFLAGS) -o test.x $(OBJFILES)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<

cleanLogs:
	rm -rf VList/*.gcda VHashTable/*.gcda *.gcda *.log *.info covinfo *.gcov

clean: cleanLogs
	rm -rf test.x *.o *.d *.gcno VList/*.o VList/*.d VList/*.gcno VHashTable/*.o VHashTable/*.d VHashTable/*.gcno

-include *.d