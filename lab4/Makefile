
TARGET = test

all: test.cpp
	g++ -o $(TARGET) test.cpp -g -Wall -lopencv_core -lopencv_highgui -lopencv_imgproc

.PHONY: clean

clean:
	-rm -rf *.o $(TARGET)
