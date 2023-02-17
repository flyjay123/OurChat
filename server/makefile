TARGET=server
SRC = $(wildcard *.cpp *.c ./dao/*.cpp ./dao/*.c)
OBJ = $(patsubst %.cpp ./dao/%.cpp *.c,%.o,$(SRC))
DEFS =
CFLAGS = -g
CC =g++
LIBS =  -lpthread -lsqlite3
$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) $(DEFS) -o $@ $^ $(LIBS)
.PHONY:
clean:
	rm -rf *.o $(TARGET)