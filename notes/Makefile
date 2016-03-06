INCLUDE_DIRS = 
CFLAGS = -g -std=c++11

all:	test test2 test3 binarytree linkedlist dp queuestack hashheaptrie threads

test: 	test.o
		g++ -o test test.o -lpthread
test.o:	test.cpp test.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c test.cpp 

test2:	test2.o
		g++ -o test2 test2.o -lpthread
test2.o:	test2.cpp test2.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c test2.cpp 

test3:	test3.o
		g++ -o test3 test3.o -lpthread
test3.o:	test3.cpp test3.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c test3.cpp 

test4:	test4.o
		g++ -o test4 test4.o -lpthread
test4.o:	test4.cpp test4.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c test4.cpp 

binarytree:	binarytree.o
		g++ -o binarytree binarytree.o -lpthread
binarytree.o:	binarytree.cpp binarytree.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c binarytree.cpp 

linkedlist:	linkedlist.o
		g++ -o linkedlist linkedlist.o -lpthread
linkedlist.o:	linkedlist.cpp linkedlist.h
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c linkedlist.cpp 

dp:	dp.o
		g++ -o dp dp.o -lpthread
dp.o:	dp.cpp 
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c dp.cpp 

queuestack:	queuestack.o
		g++ -o queuestack queuestack.o -lpthread
queuestack.o:	queuestack.cpp 
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c queuestack.cpp 

hashheaptrie:	hashheaptrie.o
		g++ -o hashheaptrie hashheaptrie.o -lpthread
hashheaptrie.o:	hashheaptrie.cpp 
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c hashheaptrie.cpp 

threads:	threads.o
		g++ -o threads threads.o -lpthread
threads.o:	threads.cpp 
		g++ $(INCLUDE_DIRS) $(CFLAGS) -c threads.cpp 

clean:		
		rm -f test test.exe test.o test2 test2.exe test2.o test3 test3.exe test3.o test4 test4.exe test4.o binarytree binarytree.exe binarytree.o linkedlist linkedlist.exe linkedlist.o dp dp.exe dp.o queuestack queuestack.exe queuestack.o hashheaptrie hashheaptrie.exe hashheaptrie.o threads threads.exe threads.o 

