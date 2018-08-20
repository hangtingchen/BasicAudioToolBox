all : hmath.o sigProcess.o WAVE.o fft.o fileIO.o

hmath.o:hmath.cpp hmath.hpp
	g++ -c hmath.cpp

sigProcess.o:sigProcess.cpp sigProcess.hpp hmath.hpp
	g++ -c sigProcess.cpp

WAVE.o:WAVE.cpp WAVE.hpp hmath.hpp
	g++ -c WAVE.cpp

fft.o:fft.cpp fft.hpp hmath.hpp
	g++ -c fft.cpp

fileIO.o:fileIO.cpp fileIO.hpp
	g++ -c fileIO.cpp

clean:
	rm -f *.o
