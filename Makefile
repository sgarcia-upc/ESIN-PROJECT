OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

all: test
    
contenidor: clean
	g++ -c $(OPCIONS) contenidor.cpp

ubicacio: clean
	g++ -c $(OPCIONS) ubicacio.cpp
	
ubicacio_tester: clean ubicacio
	g++ $(OPCIONS) -o ubicacio.o ubicacio_tester.cpp -lesin -o ubicacio_tester.exe
	./ubicacio_tester.exe > meh
	diff -q meh ubicacio_tester.cor
	rm meh

contenidor_tester: clean contenidor
	g++ $(OPCIONS) -o contenidor.o contenidor_tester.cpp -lesin -o contenidor_tester.exe	
	./contenidor_tester.exe > meh
	diff -q meh contenidor_tester.cor
	rm meh
		
test: ubicacio_tester contenidor_tester

clean:
	rm -f *.exe
	rm -f *.o
