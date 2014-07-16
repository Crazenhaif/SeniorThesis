Particle.o:	Particle.h Particle.cpp
	g++ -c Particle.cpp

arrayconstructor.o:	arrayconstructor.h arrayconstructor.cpp
	g++ -c arrayconstructor.cpp

BinData.o:	BinData.cpp BinData.h
	g++ -c BinData.cpp

Halo.o:	halo.h Halo.cpp
	g++ -c Halo.cpp

xorshift.o:	xorshift.cpp xorshift.h
	g++ -c xorshift.cpp

data:	arrayconstructor.o BinData.o Particle.o xorshift.o Halo.o Main.cpp
	g++ -o data arrayconstructor.o BinData.o Particle.o xorshift.o Halo.o Main.cpp -lpthread

clean:
	rm -rf *.o
