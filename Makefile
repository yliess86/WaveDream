RM=rm -rf
MKDIR=mkdir -p

CXX=g++ -std=c++17
LDLIBS=-lsoundio -lpthread
SHARED=-shared
FPIC=-fPIC -g

MAIN=src/main.cpp
IMPORT=src/import.cpp
BINDING=src/binding.cpp

IMPORT_OBJ=build/import.o
EXE=build/wavedream
SO=lib/libwavedream.so

PYBIND_INCLUDES=`python3 -m pybind11 --includes`
PYBIND_CONFIG=`python3-config --extension-suffix`
PYBIND_LIB=lib/wavedream

build:
	$(MKDIR) build
	$(CXX) -o $(EXE) $(MAIN) $(LDLIBS)

shared:
	$(MKDIR) lib
	$(MKDIR) build
	$(CXX) $(FPIC) -o $(IMPORT_OBJ) $(IMPORT)
	$(CXX) -o $(SO) $(IMPORT_OBJ) $(SHARED) $(LIBS)

module:
	$(MKDIR) lib
	$(CXX) -O3 -Wall $(SHARED) $(FPIC) \
		$(PYBIND_INCLUDES) \
		$(BINDING) -o $(PYBIND_LIB) \
		$(PYBIND_CONFIG)

clean:
	$(RM) build
	$(RM) lib

all: clean build shared