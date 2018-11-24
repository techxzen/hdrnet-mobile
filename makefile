
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
LIB_DIR = ./lib
BIN_DIR = ./bin

UTILS_SRC = $(wildcard ${SRC_DIR}/utils/*.cpp)  
UTILS_OBJ = $(patsubst %.cpp,${OBJ_DIR}/%.o,$(notdir ${UTILS_SRC})) 

HDRNET_SRC = $(wildcard ${SRC_DIR}/hdrnet/*.cpp)  
HDRNET_OBJ = $(patsubst %.cpp,${OBJ_DIR}/%.o,$(notdir ${HDRNET_SRC})) 

CNN_SRC = $(wildcard ${SRC_DIR}/cnn/*.cpp)  
CNN_OBJ = $(patsubst %.cpp,${OBJ_DIR}/%.o,$(notdir ${CNN_SRC})) 

SO_NAME = hdrnet_api
SO_FILE = $(LIB_DIR)/lib$(SO_NAME).so

TARGET = main
BIN_TARGET = ${BIN_DIR}/${TARGET}
BIN_FILE = $(BIN_DIR)/${TARGET}.cpp


CC = g++
CFLAGS = -std=c++11 -g -Wall -fPIC -I${INC_DIR} -DMODEL_DIR=$(MODEL_DIR) -DDEBUG



# build main
${BIN_TARGET}: $(SO_FILE)
	$(CC) $(CFLAGS) $(BIN_FILE) -L $(LIB_DIR) -l$(SO_NAME) -o $@

# so file
$(SO_FILE): $(UTILS_OBJ) $(HDRNET_OBJ) $(CNN_OBJ)
	mkdir -p $(LIB_DIR)
	ld -shared $^ -o $@

# build 
${OBJ_DIR}/%.o:${SRC_DIR}/hdrnet/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c  $^ -o $@

${OBJ_DIR}/%.o:${SRC_DIR}/utils/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c  $^ -o $@


ndk-dll:
	ndk-build -C ndk/dll/jni -B

ndk-main:
	ndk-build -C ndk/main/jni -B

.PHONY:clean
clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(LIB_DIR)
	-rm $(BIN_TARGET)
	-rm *.rgb
	-rm *.rgb.jpg
	-rm *.so
	-rm script/*.pyc
	-rm -rf ndk/dll/libs
	-rm -rf ndk/dll/obj
	-rm -rf ndk/main/libs
	-rm -rf ndk/main/obj




