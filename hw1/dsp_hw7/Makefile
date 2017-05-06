.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library
CC=g++
TARGET=train test

all: $(TARGET)
# type make/make all to compile test_hmm
	$(CC) train.cpp -o train
	$(CC) test.cpp -o test
clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
