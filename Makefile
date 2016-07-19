HSA_RUNTIME_PATH?=/opt/rocm/hsa
CXX?=g++
CXXFLAGS=-Wall -Weffc++ -Ofast -std=gnu++11
INCS?=-I$(HSA_RUNTIME_PATH)/include
LDFLAGS=-L$(HSA_RUNTIME_PATH)/lib -lhsa-runtime64

all: build test

build: hsainfo

hsainfo: hsainfo.cpp
	$(CXX) $(CXXFLAGS) $(INCS) -o $@ $^ $(LDFLAGS)


test: hsainfo
	./hsainfo

clean:
	$(RM) hsainfo
