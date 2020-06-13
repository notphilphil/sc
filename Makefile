CPP=g++
CPPFLAGS=-I. -I/usr/local/include -Wall -g

debug: main.o
	$(CPP) $(CPPFLAGS) -o sc main.o

clean:
	rm -f ./sc *.o
