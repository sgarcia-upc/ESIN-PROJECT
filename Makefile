OPCIONS = -O0 -Wall -Wextra -Werror -Wno-sign-compare -std=c++11 -ansi -g

all: clean program.exe

program.exe: driver_gestterm.o ubicacio.o contenidor.o terminal.o
	g++ -o program.exe driver_gestterm.o ubicacio.o contenidor.o terminal.o -lesin -L./libesin/lib
	rm -f *.o
	
driver_gestterm.o: driver_gestterm.cpp ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep cataleg.t cataleg.hpp terminal.hpp terminal.rep
	g++ -c driver_gestterm.cpp $(OPCIONS)
	
ubicacio.o: ubicacio.cpp ubicacio.hpp ubicacio.rep
	g++ -c ubicacio.cpp $(OPCIONS)
	
contenidor.o: contenidor.cpp contenidor.hpp contenidor.rep
	g++ -c contenidor.cpp $(OPCIONS)

terminal.o: terminal.hpp terminal.cpp terminal.rep ubicacio.hpp ubicacio.rep contenidor.hpp contenidor.rep
	g++ -c terminal.cpp $(OPCIONS)

cataleg:
	g++ -c cataleg.t cataleg.hpp $(OPCIONS)

clean:
	rm -f *.o
	rm -f *.exe
	rm -f *.gch
	
