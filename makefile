all: example lib1.so lib2.so

example: int_test.o
	g++ -std=c++11 -g -Wall -ldl -lyaml-cpp -rdynamic int_test.o -o example

clean:
	find . -type f \( -name '*.so' -o -name '*.o' -o -name 'example' \) -exec rm -v {} \;

int_test.o: integrand.cpp
	g++ -std=c++11 -fPIC -g -c -Wall int_test.cpp

lib1.o: lib1.cpp
	g++ -std=c++11 -fPIC -g -c -Wall lib1.cpp

lib1.so: lib1.o
	g++ -std=c++11 -shared -o lib1.so lib1.o

lib2.o: lib2.cpp
	g++ -std=c++11 -fPIC -g -c -Wall lib2.cpp

lib2.so: lib2.o
	g++ -std=c++11 -shared -o lib2.so lib2.o
