Particle.o:	Particle.h Particle.cpp
	g++ -c Particle.cpp

arrayconstructor.o:	arrayconstructor.h arrayconstructor.cpp
	g++ -c arrayconstructor.cpp

extrasw2.o:	extrasw2.h extrasw2.cpp
	g++ -c extrasw2.cpp

w2:	arrayconstructor.o extrasw2.o Particle.o w2.cpp
	g++ -o w2 arrayconstructor.o extrasw2.o Particle.o w2.cpp -lpthread

extrasmorew2.o:	extrasmorew2.h extrasmorew2.cpp
	g++ -c extrasmorew2.cpp

morew2:	arrayconstructor.o extrasmorew2.o Particle.o morew2.cpp
	g++ -o morew2 arrayconstructor.o extrasmorew2.o Particle.o morew2.cpp -lpthread

clean:
	rm -rf *.o
