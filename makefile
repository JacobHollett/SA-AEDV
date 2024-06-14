TARGET=SA-AEDV

SRC_FILES=mapMain.c mapFuncs.c moveFuncs.c

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	gcc -Wall -g -o $(TARGET) $(SRC_FILES)