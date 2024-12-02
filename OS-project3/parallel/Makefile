CC = g++
CFLAGS = -std=c++11
CF=$(STD)

all: ImageFilters.out

ImageFilters.out: main.cpp
	$(CC) $(CF) -o ImageFilters.out main.cpp -pthread

.PHONY: clean
clean:
	rm *.out
