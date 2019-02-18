PROG = crawler
CC = g++
CPPFLAGS = -Wall -g -I/usr/include/libxml2/
LDFLAGS = -lcurl -lxml2 
OBJS = Main.o Freia.o
$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)
Main.o :
	$(CC) -c main.cpp $(CPPFLAGS)
Freia.o : Freia.h
	$(CC) -c freia.cpp $(CPPFLAGS)
clean:
	rm -f core $(PROG) $(OBJS)

#curlTest:
	#g++ curlTest.cpp -lcurl -I/usr/include/libxml2/ -lxml2 -o curl.exe & curl.exe & rm curl.exe