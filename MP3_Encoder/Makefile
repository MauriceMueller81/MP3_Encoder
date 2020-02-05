BUILD_MODE = debug

ifeq ($(BUILD_MODE),debug)
	CXXFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CXXFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

#INCLUDES = -I../include

#LDFLAGS = -L../externals

#LIBS := -lmp3lame -lpthread

# define the C source files
#SRCS =  encoder.cpp CApplinpput.cpp CEncoderLogger.cpp CInputWaveFile.cpp CLameEncoder.h COutputEncodedFile.cpp

CXX      := g++
CXXFLAGS := -pedantic-errors -Wall -Wextra
LDFLAGS  := -L../../inlcude -lmp3lame -lpthread
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := MP3Encoder
INCLUDE  := -I../../include
SRC      :=                      \
   $(wildcard src/encoder.cpp) \
   $(wildcard src/CApplinpput.cpp) \
   $(wildcard src/CEncoderLogger.cpp) \
   $(wildcard src/CInputWaveFile.cpp) \
   $(wildcard src/CLameEncoder.cpp) \
   $(wildcard src/COutputEncodedFile.cpp) 

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*