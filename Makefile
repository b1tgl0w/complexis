all: complexis

clean:
	rm main.o combined.o complexis

complexis: combined.o
	g++ --std=c++11 combined.o -o complexis -lSDL2 -lSDL2_gfx

combined.o:main.cc
	cat sample7 main.cc > combined.cc
	g++ --std=c++11 -c combined.cc

