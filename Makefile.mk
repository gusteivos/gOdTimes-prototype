
BUILD ?= debug

ifeq ($(OS),Windows_NT)
	OBJ=.obj
	EXE=.exe
	SO=.dll
	RM=cmd /C "$(dir $(abspath $(lastword $(MAKEFILE_LIST))))rm.bat" -f -v
	OS_NAME=Windows
else
	OBJ=.o
	EXE=.out
	RM=rm -rf -v
	UNAME_S:=$(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		SO=.dylib
		OS_NAME=Darwin
	else
		SO=.so
		OS_NAME=Linux_BSD
	endif
endif

ifeq ($(OS_NAME),Windows)
WINDRES:=windres
WINDRES_FLAGS:=
endif

CC:=gcc
CC_FLAGS:=-pedantic -pedantic-errors -Wall -Wextra

ifeq ($(BUILD),debug)
	CC_BUILD=-g -O0 -DDEBUG
else ifeq ($(BUILD),release)
	CC_BUILD=-O2 -DNDEBUG
else ifeq ($(BUILD),native-debug)
	CC_BUILD=-g -O0 -march=native -DDEBUG
else ifeq ($(BUILD),native-release)
	CC_BUILD=-O2 -march=native -DNDEBUG
else
	$(error Unknown BUILD type: '$(BUILD)')
endif

ifndef MAKEFILE_MK_INCLUDED

TARGET:=gOdTimes-prototype$(EXE)

INCS=
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=$(OBJ))
LIBS=-lm -lSDL2main -lSDL2 -llua5.3

ifeq ($(OS_NAME),Windows)
	gOdTimes_rc:=gOdTimes.rc
	gOdTimes_rc_OBJ=$(gOdTimes_rc)$(OBJ)
	OBJS+=$(gOdTimes_rc_OBJ)
endif

$(TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) $(CC_BUILD) -o $@ $^ $(LIBS)

run: $(TARGET)
	./$(TARGET)

%$(OBJ): %.c
	$(CC) $(CC_FLAGS) $(CC_BUILD) $(INCS) -c $< -o $@ $(LIBS)

ifeq ($(OS_NAME),Windows)
$(gOdTimes_rc_OBJ): $(gOdTimes_rc)
	$(WINDRES) $(WINDRES_FLAGS) -O coff -i $< -o $@
endif

clean:
	$(RM) $(TARGET) $(OBJS)

endif
