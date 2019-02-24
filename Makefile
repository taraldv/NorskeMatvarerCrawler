PROG = crawler
CC = g++
CPPFLAGS = -Wall -ggdb -I/usr/include/libxml2/
LDFLAGS = -lcurl -lxml2 
OBJS = Main.o Request.o Freia.o Tine.o Parser.o Table.o Nettside.o
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
Parser.o : Parser.h
	$(CC) -c Parser.cpp $(CPPFLAGS)
Table.o : Table.h
	$(CC) -c Table.cpp $(CPPFLAGS)
Nettside.o : Nettside.h
	$(CC) -c Nettside.cpp $(CPPFLAGS)
clean:
	rm -f *stackdump $(PROG) $(OBJS)