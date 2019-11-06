all: 
	g++ -pthread thread-sort.cpp -o bubble-sort.out
	g++ -pthread thread-search.cpp -o search.out
	g++ -pthread select-sort.cpp -o select-sort.out
	g++ -pthread threads-classes.cpp -o classes.out
	g++ -pthread insert-sort-best.cpp -o insert-sort.out
