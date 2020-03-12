RM=rm -rf
MKDIR=mkdir -p

CXX=g++ -std=c++17
LDLIBS=-lsoundio -lpthread
SHARED=-shared
FPIC=-fPIC -g

MAIN=src/main.cpp
IMPORT=src/import.cpp

IMPORT_OBJ=build/import.o
EXE=build/wavedream
SO=lib/libwavedream.so

build:
	$(MKDIR) build
	$(CXX) -o $(EXE) $(MAIN) $(LDLIBS)

shared:
	$(MKDIR) lib
	$(MKDIR) build
	$(CXX) $(FPIC) -o $(IMPORT_OBJ) $(IMPORT)
	$(CXX) -o $(SO) $(IMPORT_OBJ) $(SHARED) $(LIBS)

clean:
	$(RM) build
	$(RM) lib

all: clean build shared