Halo.o:	halo.h Halo.cpp
	g++ -c Halo.cpp

Particle.o:	Particle.h Particle.cpp
	g++ -c Particle.cpp

arrayconstructor.o:	arrayconstructor.h arrayconstructor.cpp
	g++ -c arrayconstructor.cpp

equilibrium:	arrayconstructor.o Halo.o Particle.o equilibrium.cpp
	g++ -o equilibrium arrayconstructor.o Halo.o Particle.o equilibrium.cpp -lpthread

clean:
	rm -rf *.o
