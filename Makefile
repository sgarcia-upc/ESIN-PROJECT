OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

ubicacio:
	g++ -c $(OPCIONS) ubicacio.cpp
	
main: ubicacio
	g++ -o ubicacio.o main.cpp -lesin -o main
	
clean:
	rm -f main
	rm -f *.o
