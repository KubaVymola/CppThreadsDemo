all: 
	g++ -pthread thread-sort.cpp -o bubble-sort.out
	g++ -pthread thread-search.cpp -o search.out
	g++ -pthread select-sort.cpp -o select-sort.out
