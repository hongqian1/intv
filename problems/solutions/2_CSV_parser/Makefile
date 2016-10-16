INCLUDE_DIRS = 
CFLAGS = -g -std=c++11

all:	test 

test: 	test.o
		g++ -o test test.o -lpthread
test.o:	test.cpp
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c test.cpp 

clean:		
		rm -f test test.exe test.o test.exe.stackdump

