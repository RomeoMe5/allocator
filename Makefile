main: keyLoggerInt.o
	gcc -o keyLoggerInt keyLoggerInt.o -lrt

keylogger.o: keyLoggerInt.c
	gcc -c keyLoggerInt.c

clean:
	rm keyLoggerInt.o keyLoggerInt
