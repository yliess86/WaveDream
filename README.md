# WaveDream

## Install

Install all requirements (may require sudo):
```bash
$ pip3 install cython pybind11 cppimport
$
$ git clone https://github.com/andrewrk/libsoundio
$ cd libsoundio
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

Build library:
```bash
$ # Clean build directories
$ make clean
$
$ # Build example .build/wavedream
$ make build
$
$ # Build shared obj lib/libwavedream.so
$ make shared
$
$ # Build python3 module lib/wavedream.cpython-38-x86_64-linux-gnu.so
$ make module
```

## Usage

After install and compiling the python module:
- **See `test_440.py file` example**