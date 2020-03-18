#pragma once

#include <vector>

#include "delay.hpp"
#include "allpassfilter.hpp"
#include "lowpasssinglepole.hpp"

namespace wavedream {
    
    template<typename T>
    class Reverb {
        private:
            int _sr;
            T _wet;
            T _dry;

            std::vector<Delay<T>> _combs;
            std::vector<LowPassSinglePole<T>> _lowpassfilters;
            std::vector<AllPassFilter<T>> _allpassfilters;

        public:
            Reverb(int sr, T wet);

            T GetWet(void) { return this->_wet; }
            void SetWet(T wet) { this->_wet = wet; this->_dry = (T) 1.0 - wet; }

            T Output(T signal);
    };

    template<typename T>
    Reverb<T>::Reverb(int sr, T wet): _sr(sr), _wet(wet), _dry((T) 1.0 - wet) {
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03531, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03667, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03381, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03224, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02896, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03075, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02694, (T) 0.84));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02531, (T) 0.84));

        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.00510, (T) 0.50));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.01261, (T) 0.50));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.01000, (T) 0.50));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.00773, (T) 0.50));

        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
    }

    template<typename T>
    T Reverb<T>::Output(T signal) {
        T out = (T) 0.0;
        
        long unsigned int n = (long unsigned int) (this->_combs.size() * 0.5);
        for(long unsigned int i = 0; i < 2; i++) {
            T _out = (T) 0.0;
            for(long unsigned int j = i * n; j < i * n + n; j++ ) {
                Delay<T> &comb = this->_combs[j];
                _out += comb.Output(signal);
            }
            out += _out;
        }

        for(long unsigned int i = 0; i < this->_allpassfilters.size(); i++) {
            LowPassSinglePole<T> &lowpassfilter = this->_lowpassfilters[i];
            AllPassFilter<T> &allpassfilter = this->_allpassfilters[i];
            out = allpassfilter.Output(lowpassfilter.Output(out));
        }

        return this->_dry * signal + this->_wet * out;
    }

}