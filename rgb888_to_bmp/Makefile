#
# My Makefile for MingGW32 project.
#
#RM = del
RM = rm

#=== Compiler settings
#TOOLCHAIN = /c/Mingw64/mingw64/bin
TOOLCHAIN =
CC		= $(TOOLCHAIN)gcc
CXX		= $(TOOLCHAIN)g++
#=== Ar settings to build the library
AR	    = $(TOOLCHAIN)ar
ARFLAGS = rcsv

#=== Source Path
INC_DIR		= ../common
EXEC_DIR	= ../exec

#=== Project programs
GOAL = rgb888_to_bmp

C_SRCS = main.c bin2bmp.c
CPP_SRCS = $(INC_DIR)/common.cpp

C_OBJS = $(C_SRCS:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)
OBJS = $(C_OBJS) $(CPP_OBJS)


INC_FLAGS	= -I. -I$(INC_DIR)

CFLAGS	= -O2 -Wall
CFLAGS	+= -Wno-maybe-uninitialized -Wno-sign-compare
CFLAGS	+= -Wno-format -Wno-strict-aliasing
CFLAGS	+= -Wno-write-string -Wno-unused-result
CFLAGS	+= -Wno-unused-variable -Wno-unused-but-set-variable
CFLAGS	+= $(INC_FLAGS)

#=== Implicit rules

#COMPILE.c=$(CC) $(CFLAGS) -c

%.o:%.c
	@(echo "")
	@(echo "[C] Compiling $< ...")
	@$(CC) $(CFLAGS) -std=c99 -c $< -o $@

%.o:%.cpp
	@(echo "")
	@(echo "[CXX] Compiling $< ...")
	@$(CC) $(CFLAGS) -c $< -o $@


all: $(GOAL)
	@echo "***** Building $(GOAL) completes successfully! ***"

$(GOAL): $(OBJS)
	@echo "linking $@ ..."
	$(CXX) -o $(GOAL) $(OBJS)
	@echo
	@echo "....... $@ OK !...";
	@echo


.PHONY: clean check docs $(GOAL)

clean:
	$(RM) -f $(OBJS) $(GOAL) $(GOALLIB)

docs:
	@(cd doc ; $(MAKE))

install: $(GOAL)
	cp -f $(GOAL) $(EXEC_DIR)

sinclude $(OBJS:.o=.d)
