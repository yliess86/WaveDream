# WaveDream

WaveDream is an experimental Virtual Analog Synthetizer. It is developped using C++ for performance and compiled as a python module using the pybind11 library for easy usage.

Here is a list of the features already developed or in developement for the project:
- [x] Audio libsoundio Wrapper
- [x] Oscilators
    - [x] Sin
    - [x] Square
    - [x] Triangle
    - [x] Analog Sawtooth (Preserve harmonics)
    - [x] White Noise
- [x] Formant
- [x] Timbre
- [x] ADSR Enveloppe
- [x] LFO modulation
- [x] Note
- [x] Instrument
- [x] FXs
    - [x] Reverb
    - [x] Delay
    - [x] Distortion
    - [x] Filters
        - [x] All Pass Filter
        - [x] Low Pass Single Pole Filter
        - [x] Low Pass Filter 
        - [x] High Pass Filter 
        - [x] Band Pass Filter 
- [ ] Modules
    - [x] Arpegiator
    - [ ] Sequencer
    - [x] Random Seed
- [ ] Inputs
    - [ ] Keyboard Inputs
    - [ ] MIDI Controller

Known issues:
- [ ] Python module does not always quite when `audio.stop()` is called
- [ ] May Initialize callback after `audio.Init()` to get *sample rate*
- [ ] Need better filter implementation

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
$ (sudo) make install
```

Install library (may require sudo):
```bash
$ # Install inplace
$ pyton3 setup.py build_ext --inplace
$
$ # Install
$ (sudo) python3 setup.py install
$
$ # Uninstall
$ (sudo) pip3 remove wavedream
```

## Usage

After install and compiling the python module you'll find some example usage of the library in the `examples` folder.
Example from the `example.py` can be listent at [soundcloud](https://soundcloud.com/yliess-hati/wavedream-demo).

## Dependencies

- libsoundio [SoundIO](http://libsound.io/)
- pybind11 [Pybind11](https://github.com/pybind/pybind11)