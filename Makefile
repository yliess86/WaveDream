RM=rm -rf
MKDIR=mkdir

CXX=g++ -std=c++17
LDLIBS=-lsoundio -lpthread
SHARED=-shared
FPIC=-fPIC

HEADERS=$(lsforeach header, $(wildcard src/*.hpp), $(header);)
HEADERS_OBJ=$(lsforeach header, $(wildcard $(HEADERS)), $(header:.hpp=.o);)
HEADERS_OBJ=$(lsforeach header, $(wildcard $(HEADERS)), $(header:.src=.build);)

SRCS=src/*.cpp

EXE=wavedream

SO_COMP=libwavedream.c
SO=libwavedream.so

build:
	$(MKDIR) build
	$(CXX) $(SRCS) -o build/$(EXE) $(LDLIBS)

shared:
	$(MKDIR) lib
	$(MKDIR) build
	$(CXX) $(FPIC) -c -o $(HEADERS_OBJ) $(HEADERS)

clean:
	$(RM) build
	$(RM) lib