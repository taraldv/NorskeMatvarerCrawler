PROG = crawler
CC = g++
CPPFLAGS = -Wall -ggdb -I/usr/include/libxml2/
LDFLAGS = -lcurl -lxml2 
OBJS = Main.o Request.o Freia.o Tine.o Parser.o Table.o Nettside.o QMeieriene.o
$(PROG) : $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)
Main.o :
	$(CC) -c Main.cpp $(CPPFLAGS)

Request.o : http/Request.h
	$(CC) -c http/Request.cpp $(CPPFLAGS)
Parser.o : http/Parser.h
	$(CC) -c http/Parser.cpp $(CPPFLAGS)
Table.o : http/Table.h
	$(CC) -c http/Table.cpp $(CPPFLAGS)

Nettside.o : nettsider/Nettside.h
	$(CC) -c nettsider/Nettside.cpp $(CPPFLAGS)
Freia.o : nettsider/Freia.h
	$(CC) -c nettsider/Freia.cpp $(CPPFLAGS)
Tine.o : nettsider/Tine.h
	$(CC) -c nettsider/Tine.cpp $(CPPFLAGS)
QMeieriene.o : nettsider/QMeieriene.h
	$(CC) -c nettsider/QMeieriene.cpp $(CPPFLAGS)
	
clean:
	rm -f *stackdump $(PROG) $(OBJS)