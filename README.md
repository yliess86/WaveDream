# WaveDream

WaveDream is an experimental Virtual Analog Synthetizer. It is developped using C++ for performance and compiled as a python module using the pybind11 library for easy usage.

Here is a list of the features already developed or in developement for the project:
- [x] Audio libsoundio Wrapper
- [x] Formant
- [x] Timbre
- [x] ADSR Enveloppe
- [ ] LFO modulation
- [x] Note
- [x] Instrument
- [ ] FXs
    - [ ] Reverb
    - [ ] Delay
    - [ ] Distortion
    - [ ] Filters
- [ ] Inputs
    - [ ] Keyboard Inputs
    - [ ] MIDI Controller

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

After install and compiling the python module you'll find some example usage of the library in the `examples` folder.