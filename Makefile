CC = g++
BINDIR = ./bin
SRCDIR = ./src
TARGET = ./bin/main
SRCS = $(wildcard $(SRCDIR)/*.cpp)

VERSION = -std=c++17

SDL_INCLUDE = -IC:/SDL2-2.0.12/include/
HEADER_INCLUDE = -I./src/header
MAGIC_ENUM_INCLUDE = -IC:\SDK\magic_enum\include\magic_enum

INCDIR  = $(SDL_INCLUDE) $(HEADER_INCLUDE) $(MAGIC_ENUM_INCLUDE)
LIBDIR  = -LC:/SDL2-2.0.12/lib/x86/
LIBS    = -lSDL2main -lSDL2

.PHONY: run

$(TARGET): 
	g++ -std=c++17 $(INCDIR) $(LIBDIR) $(SRCS) $(LIBS) -o $(TARGET) -O3 

run: $(TARGET)
	$(TARGET)