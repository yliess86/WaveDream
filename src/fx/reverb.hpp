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
            T _gain;
            T _feedback;
            T _wet;
            T _dry;

            std::vector<LowPassSinglePole<T>> _lowpassfilters;
            std::vector<Delay<T>> _combs;
            std::vector<AllPassFilter<T>> _allpassfilters;

        public:
            Reverb(int sr, T gain, T feedback, T wet);
            Reverb(int sr, T gain, T wet): Reverb(sr, gain, (T) 0.84, wet) {}

            T GetGain(void) { return this->_gain; }
            void SetGain(T gain) { this->_gain = gain; }

            T GetFeedback(void) { return this->_feedback; }
            void SetFeedback(T feedback) { this->_feedback = feedback; }

            T GetWet(void) { return this->_wet; }
            void SetWet(T wet) { this->_wet = wet; this->_dry = (T) 1.0 - wet; }

            T Output(T signal);
    };

    template<typename T>
    Reverb<T>::Reverb(int sr, T gain, T feedback, T wet): 
        _sr(sr), _gain(gain), _feedback(feedback), _wet(wet), _dry((T) 1.0 - wet) {
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));
        this->_lowpassfilters.push_back(LowPassSinglePole<T>(.2));

        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03531, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03667, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03381, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03224, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02896, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.03075, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02694, (T) this->_feedback));
        this->_combs.push_back(                 Delay<T>(sr, (T) 0.02531, (T) this->_feedback));

        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.00510, (T) (this->_feedback * 0.59523)));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.01261, (T) (this->_feedback * 0.59523)));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.01000, (T) (this->_feedback * 0.59523)));
        this->_allpassfilters.push_back(AllPassFilter<T>(sr, (T) 0.00773, (T) (this->_feedback * 0.59523)));
    }

    template<typename T>
    T Reverb<T>::Output(T signal) {
        signal = signal * this->_gain;
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