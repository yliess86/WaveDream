#pragma once

#include <cmath>

#include "const.hpp"
#include "clock.hpp"

namespace wavedream {

    enum OcillatorType { SIN, SQUARE, TRIANGLE, SAWTOOTH, NOISE };

    template<typename T>
    class Oscillator {
        private:
            int _osc;

            T _W(T freq) { return TWO_PI * freq; }

        public:
            Oscillator(int osc) : _osc(osc) {}

            T Output(T time, T freq);
    };

    template<typename T>
    T Oscillator<T>::Output(T time, T freq) {
        T w = this->_W(freq);

        switch (this->_osc) {
            case SIN:
                return sin(w * time);

            case SQUARE:
                return sin(w * time) > 0 ? 1.0: -1.0;

            case TRIANGLE:
                return asin(sin(w * time)) * PI_O_TWO;

            case SAWTOOTH: {
                
                T out = 0.0;
                for (T n = 1.0; n < 50.0; n++)
                    out += sin(n * w * time) / n;

                return out * TWO_O_PI;
            }

            case NOISE:
                return 2.0 * ((T) rand() / (T) RAND_MAX) - 1.0;

            default:
                return (T) 0.0;
        }
    }

}
