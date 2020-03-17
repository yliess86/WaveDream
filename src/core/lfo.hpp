#pragma once

#include <cmath>

#include "osc.hpp"

namespace wavedream {

    template<typename T>
    class LFO {
        private:
            T _amount;
            T _rate;
            Oscillator<T>* _osc;

        public:
            LFO(int osc, T amount, T rate);

            T GetAmount(void) { return this->_amount; }
            void SetAmount(T amount) { this->_amount = amount; }

            T GetRate(void) { return this->_rate; }
            void SetRate(T rate) { this->_rate = std::min(std::max(rate, (T) 20.0), (T) 0.1); }

            Oscillator<T>* GetOsc(void) { return this->_osc; }
            void SetOsc(int osc) { this->_osc = new Oscillator<T>(osc); }

            T Output(T time);
    };

    template<typename T>
    LFO<T>::LFO(int osc, T amount, T rate): _amount(amount) {
        this->_amount = std::min(std::max(rate, (T) 20.0), (T) 0.1);
        this->_osc = new Oscillator<T>(osc);    
    }

    template<typename T>
    T LFO<T>::Output(T time) {
        return this->_amount * this->_osc->Output(time, this->_rate);
    }

}