# Shell path below if running on Windows through Git Bash
# SHELL = '/c/Program Files/Git/usr/bin/bash'
CC = g++
SRC = src
OBJ = $(SRC)/obj
INC = include
BIN = bin
AI_MAIN = $(SRC)/main.cpp
EYE_MAIN = VisionTest.cpp
EYE_AI_MAIN = VisionAI.cpp
MAIN = Jelly.cpp
AI_OBJ_DEPS = $(OBJ)/Piece.o $(OBJ)/Board.o $(OBJ)/AI.o
EYE_OBJ_DEPS = $(OBJ)/Vision.o
ARM_OBJ_DEPS = $(OBJ)/robotArm.o
CFLAGS = -O2 -I$(INC) -Wall -o
DEBUG = #-g
EYE_LIBS = -lopencv_imgcodecs -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -I/usr/local/include/ -lraspicam -lraspicam_cv
ARM_LIBS = -lzmq
# Defining nullptr as NULL for EECS Linux servers
EECS = #-DNO_C11
C11 = -std=c++11

all : JellyAI VisionTest

Jelly : $(EYE_OBJ_DEPS) $(AI_OBJ_DEPS) $(ARM_OBJ_DEPS)
	$(CC) $^ $(MAIN) $(DEBUG) $(EECS) $(C11) $(CFLAGS) $(BIN)/$@ $(EYE_LIBS) $(ARM_LIBS)

VisionAI : $(EYE_OBJ_DEPS) $(AI_OBJ_DEPS)
	$(CC) $^ $(EYE_AI_MAIN) $(DEBUG) $(EECS) $(C11) $(CFLAGS) $(BIN)/$@ $(EYE_LIBS)

VisionTest : $(EYE_OBJ_DEPS) $(AI_OBJ_DEPS)
	$(CC) $^ $(EYE_MAIN) $(DEBUG) $(EECS) $(C11) $(CFLAGS) $(BIN)/$@ $(EYE_LIBS)

JellyAI : $(AI_OBJ_DEPS)
	$(CC) $^ $(AI_MAIN) $(DEBUG) $(EECS) $(C11) $(CFLAGS) $(BIN)/$@

$(OBJ)/Vision.o : $(SRC)/Vision.cpp
	$(CC) -c $< $(DEBUG) $(EECS) $(C11) $(EYE_LIBS) $(CFLAGS) $@

$(OBJ)/robotArm.o : $(SRC)/robotArm.cpp
	$(CC) -c $< $(DEBUG) $(EECS) $(C11) $(ARM_LIBS) $(CFLAGS) $@

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -c $< $(DEBUG) $(EECS) $(C11) $(CFLAGS) $@

.PHONY : clean

clean :
	rm -f $(OBJ)/*.o $(BIN)/* $(SRC)/*.jpg
