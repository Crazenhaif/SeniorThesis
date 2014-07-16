fitting.o:	fitting.h fitting.cpp
	g++ -c fitting.cpp

arrayconstructor.o:	arrayconstructor.h arrayconstructor.cpp
	g++ -c arrayconstructor.cpp

filestuff.o:	filestuff.cpp filestuff.h
	g++ -c filestuff.cpp

xorshift.o:	xorshift.cpp xorshift.h
	g++ -c xorshift.cpp

fit:	arrayconstructor.o fitting.o filestuff.o xorshift.o Main.cpp
	g++ -o fit arrayconstructor.o fitting.o filestuff.o xorshift.o Main.cpp -lpthread

clean:
	rm -rf *.o
