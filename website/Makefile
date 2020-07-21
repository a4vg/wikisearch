CXX = g++
CXXVERSION = --std=c++11

LDIR = ./lib
DP_LDIR = ../DataProcessing/lib
DP_IDIR = ../DataProcessing/include
CXXFLAGS = -I$(LDIR) -I$(DP_IDIR) -I$(DP_LDIR)
LDFLAGS = -lzim -lmyhtml -pthread

DP_SRCDIR = ../DataProcessing/src
SRCFILES = $(wildcard $(DP_SRCDIR)/*.cpp)
OBJFILES = $(SRCFILES:.c=.o)

DATADIR = ../DataProcessing/data/

server : $(OBJFILES)
	$(CXX) $(CXXVERSION) $(CXXFLAGS) $^ $@.cpp -o $@ $(LDFLAGS) ;\
	export DATA_DIR=$(DATADIR) ;