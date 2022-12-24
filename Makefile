CC = g++
CFLAGS =
BINDIR = ./bin
SRCDIR = ./src
TARGET = ./bin/main
SRCS = $(wildcard $(SRCDIR)/*.cpp)
INCDIR  = -IC:/SDL2-2.0.12/include/ -I./src/header
LIBDIR  = -LC:/SDL2-2.0.12/lib/x86/
LIBS    = -lSDL2main -lSDL2

.PHONY: all
all:
	g++ $(INCDIR) $(LIBDIR) $(SRCS) $(LIBS) -o $(TARGET)