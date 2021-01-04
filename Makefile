OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

all: clean ubicacio.o contenidor.o

program.exe: driver_gesterm.o ubicacio.o contenidor.o terminal.o
	g++ program.exe driver_gesterm.o ubicacio.o contenidor.o terminal.o -lesin
	rm -f *.o
	
driver_gesterm.o: driver_gesterm.cpp ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep cataleg.t cataleg.hpp terminal.hpp terminal.rep
	g++ -c driver_gesterm.cpp $(OPCIONS)
	
ubicacio.o: ubicacio.cpp ubicacio.hpp ubicacio.rep
	g++ -c ubicacio.cpp $(OPCIONS)
	
contenidor.o: contenidor.cpp contenidor.hpp contenidor.rep
	g++ -c contenidor.cpp $(OPCIONS)

terminal.o: terminal.hpp terminal.cpp terminal.rep ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep
	g++ -c terminal.cpp $(OPCIONS)

clean:
	rm -f *.o
	rm -f *.exe
	rm -f *.gch
	
