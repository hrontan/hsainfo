HSA_RUNTIME_PATH=/opt/hsa
HSA_LLVM_PATH=/opt/amd/cloc/bin
export LD_LIBRARY_PATH=$(HSA_RUNTIME_PATH)/lib
CXX?=g++
CXXFLAGS=-Wall -Weffc++ -Ofast -std=gnu++11
INCS=-I$(HSA_RUNTIME_PATH)/include
LDFLAGS=-L$(HSA_RUNTIME_PATH)/lib -lhsa-runtime64 -lm

all: build test

build: hsainfo

hsainfo: hsainfo.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LDFLAGS)


test: hsainfo
	./hsainfo

clean:
	$(RM) hsainfo
