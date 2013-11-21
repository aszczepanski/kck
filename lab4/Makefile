
TARGET = test

all: test.o image_processing.o
	g++ -o $(TARGET) test.o image_processing.o -g -Wall -lopencv_core -lopencv_highgui -lopencv_imgproc

test.o: test.cpp image_processing.h
	g++ -o test.o -c test.cpp -g -Wall

image_processing.o: image_processing.cpp image_processing.h
	g++ -o image_processing.o -c image_processing.cpp -g -Wall

.PHONY: clean

clean:
	-rm -rf *.o $(TARGET)
	-rm -rf xxx*
