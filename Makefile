TARGETS = test main

# All header files (including template implementations)
HEADERS = dynamic_array.hpp dynamic_array.tpp \
          linked_list.hpp linked_list.tpp \
          sequence/iterator.hpp \
          sequence/sequence.hpp \
          sequence/array_sequence.hpp sequence/array_sequence.tpp \
          sequence/list_sequence.hpp sequence/list_sequence.tpp \
          tasks.hpp

all: $(TARGETS)

test: test.cpp $(HEADERS)
	g++ -I. -o $@ $<

main: main.cpp $(HEADEERS)
	g++ -I. -o $@ $<

test_run: test
	./test

clean:
	rm -f *.o $(TARGETS)

run: main
	./main

.PHONY: all clean test_run
