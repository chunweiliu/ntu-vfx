
# target name
TARGET = GrabCut
# all src names
SRCS = main.cpp grabcut.cpp gmm.cpp 
# object corresponding to src
OBJS = $(SRCS:.cpp=.o)

# compiling related flags
OPTIMIZE =
DEBUG =
OTHERFLAG =
INCDIR= -I.
CXXFLAGS = $(INCDIR) -Wall $(OPTIMIZE) $(DEBUG) $(OTHERFLAG)

# linking related flags
LINKLIB = -lm -lcmlib-imageio

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
main.o: grabcut.h gmm.h gmminc.h
gmm.o: gmm.h gmminc.h
grabcut.o: grabcut.h gmm.h gmminc.h

