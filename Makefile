DECLARATIONS = ./sequence/sequence.h ./sequence/array_sequence.h ./sequence/list_sequence.h 

app: main.cpp $(DECLARATIONS) dynamic_array.h linked_list.h
	g++ main.cpp -o app

clean:
	rm -f *.o app

run: app
	./app

