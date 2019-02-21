PROG = crawler
CC = g++
CPPFLAGS = -Wall -g -I/usr/include/libxml2/
LDFLAGS = -lcurl -lxml2 
OBJS = Main.o Request.o Freia.o Tine.o
$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)
Main.o :
	$(CC) -c Main.cpp $(CPPFLAGS)
Request.o : Request.h
	$(CC) -c Request.cpp $(CPPFLAGS)
Freia.o : Freia.h
	$(CC) -c Freia.cpp $(CPPFLAGS)
Tine.o : Tine.h
	$(CC) -c Tine.cpp $(CPPFLAGS)
clean:
	rm -f core $(PROG) $(OBJS)