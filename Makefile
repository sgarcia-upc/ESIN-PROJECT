OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

contenidor:
	g++ -c $(OPCIONS) contenidor.cpp

ubicacio:
	g++ -c $(OPCIONS) ubicacio.cpp
	
ubicacio_tester: ubicacio
	g++ -o ubicacio.o ubicacio_tester.cpp -lesin -o ubicacio_tester.exe


contenidor_tester: contenidor
	g++ -o contenidor.o contenidor_tester.cpp -lesin -o contenidor_tester.exe	
		

test: ubicacio_tester
	./ubicacio_tester.exe > meh
	diff -q meh ubicacio_tester.cor
	rm meh

github_test:
	g++ --version

clean:
	rm -f *.exe
	rm -f *.o
