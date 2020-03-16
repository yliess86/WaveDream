#pragma once

#include <cmath>

#include "const.hpp"
#include "clock.hpp"

namespace wavedream {

    template<typename T>
    class Oscillator {
        private:
            int _osc;

            T _W(T freq) { return TWO_PI * freq; }

        public:
            enum Style { 
                SIN, 
                SQUARE, 
                TRIANGLE, 
                SAWTOOTH, 
                NOISE
            };
            
            Oscillator(int osc) : _osc(osc) {}
            int GetOsc(void) { return this->_osc; }

            T Output(T time, T freq);
    };

    template<typename T>
    T Oscillator<T>::Output(T time, T freq) {
        T w = this->_W(freq);

        switch (this->_osc) {
            case Style::SIN:
                return sin(w * time);

            case Style::SQUARE:
                return sin(w * time) > 0 ? 1.0: -1.0;

            case Style::TRIANGLE:
                return asin(sin(w * time)) * PI_O_TWO;

            case Style::SAWTOOTH: {
                
                T out = 0.0;
                for (T n = 1.0; n < 50.0; n++)
                    out += sin(n * w * time) / n;

                return out * TWO_O_PI;
            }

            case Style::NOISE:
                return 2.0 * ((T) rand() / (T) RAND_MAX) - 1.0;

            default:
                return (T) 0.0;
        }
    }

}
