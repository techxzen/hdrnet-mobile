
DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.cpp)  
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC})) 


TARGET = main
BIN_TARGET = ${DIR_BIN}/${TARGET}
BIN_OBJECT = $(DIR_OBJ)/${TARGET}.o
BIN_FILE = $(DIR_BIN)/${TARGET}.cpp


CC = g++ -std=c++11
CFLAGS = -g -Wall -I${DIR_INC}


${BIN_TARGET}: $(BIN_OBJECT) ${OBJ}
	$(CC) $(BIN_OBJECT) $(OBJ) -o $@


$(BIN_OBJECT): $(BIN_FILE)
	$(CC) $(CFLAGS) -c $(BIN_FILE) -o $(BIN_OBJECT)

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@


run:
	$(BIN_TARGET)


.PHONY:clean
clean:
	find ${DIR_OBJ} -name *.o -exec rm -rf {}

