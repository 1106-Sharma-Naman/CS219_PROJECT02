all:
	g++ -std=c++17 main.cpp parser.cpp processor.cpp -o project2

run:
	./project2

clean:
	rm -f project2