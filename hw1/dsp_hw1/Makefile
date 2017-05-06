.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library
CC=g++
TARGET=train.cpp test.cpp

all: $(TARGET)
# type make/make all to compile test_hmm
	$(CC) train.cpp -o train.out
	$(CC) test.cpp -o test.out
clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
