all: example libspencer.so libdevon.so

example: main.o
	g++ -std=c++11 -g -Wall -ldl -lyaml-cpp -rdynamic main.o -o example

clean:
	find . -type f \( -name '*.so' -o -name '*.o' -o -name 'example' \) -exec rm -v {} \;

main.o: main.cpp
	g++ -std=c++11 -fPIC -g -c -Wall main.cpp

devon.o: devon.cpp
	g++ -std=c++11 -fPIC -g -c -Wall devon.cpp

libdevon.so: devon.o
	g++ -std=c++11 -shared -o libdevon.so devon.o

spencer.o: spencer.cpp
	g++ -std=c++11 -fPIC -g -c -Wall spencer.cpp

libspencer.so: spencer.o
	g++ -std=c++11 -shared -o libspencer.so spencer.o
