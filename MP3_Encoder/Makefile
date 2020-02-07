# define the C source files
#SRCS =  encoder.cpp CApplinpput.cpp CEncoderLogger.cpp CInputWaveFile.cpp CLameEncoder.h COutputEncodedFile.cpp

CXX      := g++
CXXFLAGS := -pedantic-errors -Wall -Wextra
LDFLAGS  := -L/usr/local/lib
LIBS	 := -lmp3lame -pthread 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := MP3Encoder
INCLUDE  := -I/usr/local/include/lame -I./include
SRC      :=                      \
   $(wildcard src/COutputEncodedFile.cpp) \
   $(wildcard src/CApplinput.cpp) \
   $(wildcard src/CEncoderLogger.cpp) \
   $(wildcard src/CInputWaveFile.cpp) \
   $(wildcard src/CLameEncoder.cpp) \
   $(wildcard src/MP3Encoder.cpp) 

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

#BUILD_MODE = debug

#ifeq ($(BUILD_MODE),debug)
#	CXXFLAGS += -g
#else ifeq ($(BUILD_MODE),run)
#	CXXFLAGS += -O2
#else
#	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@printf " build directory is :`pwd` \n"
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS) $(LIBS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g3 -O2
debug: all

release: CXXFLAGS += -O3
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*