
CXX = g++
CXXFLAGS := -O2 -g -Wall -fmessage-length=0 -std=c++11
LIBS :=

SRC_DIR = src
OBJ_DIR = obj
EXE_DIR = exe

TARGET := TerminalGames

SRC := ConsoleWindow.cpp MenuDisplay.cpp SnakeGame.cpp TerminalGames.cpp
OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: build run

build: create_obj create_exe $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(EXE_DIR)/$@ $^ $(LIBS)

disp:
	@echo $(OBJ)

create_obj:
	@cmd /c if not exist $(OBJ_DIR) md $(OBJ_DIR)
	
create_exe:
	@cmd /c if not exist $(EXE_DIR) md $(EXE_DIR)
	
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
run: 
	$(EXE_DIR)/$(TARGET).exe
	