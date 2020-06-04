CPP=g++
CPPFLAGS=-I -Wall -g

debug: main.o
	$(CPP) $(CPPFLAGS) -o sc main.o

clean:
	rm sc
	rm *.o