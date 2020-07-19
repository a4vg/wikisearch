CXX = g++
CXXVERSION = --std=c++11

IDIR = ./include
LDIR = ./lib
CXXFLAGS = -I$(IDIR) -I$(LDIR)
LDFLAGS = -lzim -lmyhtml

SRCDIR = ./src
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(SRCFILES:.c=.o)

DATADIR = data/

processing : $(OBJFILES)
	$(CXX) $(CXXVERSION) $(CXXFLAGS) $^ $@.cpp -o $@ $(LDFLAGS) ;\
	export DATA_DIR=$(DATADIR) ;
