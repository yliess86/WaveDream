RM=rm -rf
MKDIR=mkdir -p

CXX=g++ -std=c++17
LDLIBS=-lsoundio -lpthread
SHARED=-shared
FPIC=-fPIC

BINDING=src/binding.cpp

PYBIND_INCLUDES=`python3 -m pybind11 --includes`
PYBIND_CONFIG=`python3-config --extension-suffix`
PYBIND_LIB=lib/wavedream

module:
	$(MKDIR) lib
	$(CXX) -O3 -Wall $(SHARED) $(FPIC) -g  $(PYBIND_INCLUDES) \
		$(BINDING) -o $(PYBIND_LIB)$(PYBIND_CONFIG) $(LDLIBS)

clean:
	$(RM) lib

all: clean module