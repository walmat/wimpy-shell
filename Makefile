wsh: main.o wsh.o command.o pCommand.o jManager.o pManager.o Foreground.o Background.o
	g++ -o wsh main.o wsh.o command.o pCommand.o jManager.o pManager.o Foreground.o Background.o

main.o: main.cpp wsh.h
	g++ -c main.cpp

wsh.o: wsh.cpp wsh.h command.h pCommand.h jManager.h pManager.h Foreground.h Background.h
	g++ -c wsh.cpp

command.o: command.cpp command.h
	g++ -c command.cpp

pCommand.o: pCommand.cpp pCommand.h command.h
	g++ -c pCommand.cpp

jManager.o: jManager.cpp jManager.h Background.h command.h
	g++ -c jManager.cpp

pManager.o: pManager.cpp pManager.h pCommand.h command.h
	g++ -c pManager.cpp

Foreground.o: Foreground.cpp Foreground.h command.h
	g++ -c Foreground.cpp

Background.o: Background.cpp Background.h command.h
	g++ -c Background.cpp

clean:
	rm -f *.o *~
