
#Makefile for vfx_final/mvc

# target name
TARGET = main

# all sources names
SRCS = main.cpp MVC.cpp
# object corresponding to src
OBJS = $(SRCS:.cpp=.o)

#TAR = main

#all:
#	g++ $(SRC) -o $(TAR) -lcmlib-imageio
#clean:
#	rm $(TAR)

# compiling related flags
OPTIMIZE =
DEBUG =
OTHERFLAG =
INCDIR= -I.
CXXFLAGS = $(INCDIR) -Wall $(OPTIMIZE) $(DEBUG) $(OTHERFLAG)

# linking related flags
LINKLIB = -lcmlib-imageio -lm

# target
all: $(TARGET)

# compile all object
%.o: %.c
	$(CXX) -c $< $(CXXFLAGS)

# compile target
$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LINKLIB)

# clean object and target
clean:
	rm -f $(OBJS) $(TARGET)

# all object including files
Vec2.o: Vec2.h
MVC.o: MVC.h
#main.o: main.h

