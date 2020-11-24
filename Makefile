ubicacio:
	g++ -c -Wall ubicacio.cpp
	
main: ubicacio
	g++ -o ubicacio.o main.cpp -lesin -o main
	
clean:
	rm -f main
	rm -f *.o
