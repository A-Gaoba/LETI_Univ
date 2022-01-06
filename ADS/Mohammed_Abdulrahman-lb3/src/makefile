all: main.o bt_implementation.o functions.o
        g++ main.o  bt_implementation.o functions.o -o a.out

main.o: main.cpp Btree.h functions.h
        g++ -c main.cpp
functions.o: functions.cpp functions.h Btree.h
        g++ -c functions.cpp

bt_implementation.o: bt_implementation.cpp Btree.h
        g++ -c bt_implementation.cpp
clean:
        rm *.o bt_implementation


