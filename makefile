CC=gcc
CFLAGS=-o 
DEPS=
TARGET=cypher

ifeq ($(shell which bleachbit), /usr/bin/bleachbit)
	DF=bleachbit -s $(TARGET) *.o
else
	DF=rm ./$(TARGET) *.o
endif

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< 

$(TARGET): cypherCrack.o
	$(CC) $(CFLAGS) cypher cypherCrack.o

clean:
	$(DF)
