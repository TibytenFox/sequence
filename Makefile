app: main.o
	g++ main.o -o app

main.o: main.cpp dynamic_array.h
	g++ -c main.cpp

clean:
	rm -f *.o app

