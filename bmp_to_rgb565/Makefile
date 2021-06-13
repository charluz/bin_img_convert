#
# My Makefile for MingGW32 project.
#

#=== Compiler settings
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
GOAL = bmp2rgb565

C_SRCS =
CPP_SRCS = bmp2rgb565.cpp $(INC_DIR)/common.cpp

C_OBJS = $(C_SRCS:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)
OBJS = $(C_OBJS) $(CPP_OBJS)


INC_FLAGS	= -I. -I$(INC_DIR)

CFLAGS	= -Wall
CFLAGS	+= $(INC_FLAGS)
CFLAGS	+= -Wno-maybe-uninitialized -Wno-sign-compare
CFLAGS	+= -Wno-format -Wno-strict-aliasing
CFLAGS	+= -Wno-write-string -Wno-unused-result
CFLAGS	+= -Wno-unused-variable -Wno-unused-but-set-variable


%.o:%.c
	@(echo "")
	@(echo "[C] Compiling $< ...")
	@$(CC) -MM -MF $(@:.o=.d)  -std=c99 $(CFLAGS) $<
	@$(cc) $(CFLAGS) -c $< -o $@ 

%.o:%.cpp
	@(echo "")
	@(echo "[CXX] Compiling $< ...")
	@$(CXX) -MM -MF $(@:.o=.d) $(CFLAGS) $<
	@$(CXX) $(CFLAGS) -c $< -o $@ 

all : $(GOAL)

$(GOAL) : $(OBJS)
	@echo ""
	@echo "Linking $@ ..."
	@$(CXX) -o $@ $^
	@echo ""
	@echo "....... $@ OK !..."
	@echo ""


.PHONY: clean install docs touch $(GOAL) all

touch:
	touch $(GOAL)

clean:
	$(RM) -f $(OBJS) *.d $(GOAL) $(GOALLIB)

docs:
	@(cd doc ; $(MAKE))

check:
	@(cd test ; $(MAKE))

install: $(GOAL)
	cp -f $(GOAL) $(EXEC_DIR)


sinclude $(OBJS:.o=.d)
