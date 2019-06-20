main : main.o tgaimage.o camera.o
	g++ -o main main.o tgaimage.o camera.o

tgaimage.o : tgaimage.h
	g++ -c tgaimage.cpp

camera.o : camera.h
	g++ -c camera.cpp

main.o : common.h line.h triangle.h camera.h 
	g++ -c main.cpp 

.PHONY : clean
clean :
	-rm main *.o