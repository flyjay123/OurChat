TARGET=server
SRC = $(wildcard *.cpp *.c)
OBJ = $(patsubst %.cpp  *.c,%.o,$(SRC))
DEFS =
CFLAGS = -g
CC =g++
LIBS =  -lpthread -lSQLiteCpp -lsqlite3 
$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) $(DEFS) -o $@ $^ $(LIBS)
.PHONY:
clean:
	rm -rf *.o $(TARGET)
