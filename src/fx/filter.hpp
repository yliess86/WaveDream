#pragma once

#include <vector>
#include <cmath>

namespace wavedream {

    template<typename T>
    class Filter {
        private:
            int _filter;
            int _order;
            T _cutoff;
            T _resonance;
            T _feedback;

            std::vector<T> _buffer;

        public:
            enum Mode {
                LOW_PASS = 0,
                HIGH_PASS,
                BAND_PASS
            };

            Filter(int filter, int order, T cutoff, T resonance);

            int GetFilter(void) { return this->_filter; }
            void SetFilter(int filter) { this->_filter = filter; }

            int GetOrder(void) { return this->_order; }
            void SetOrder(int order);
            
            T GetFeedback(void) { 
                return std::fmin(this->_resonance + this->_resonance / ((T) 1.0 - this->_cutoff), (T) 1.0); 
            }

            T GetCutOff(void) { return this->_cutoff; }
            void SetCutoff(T cutoff) { this->_cutoff = cutoff; this->_feedback = this->GetFeedback(); }
            
            T GetResonance(void) { return this->_resonance; }
            void SetResonance(T resonance) { this->_resonance = resonance; this->_feedback = this->GetFeedback(); }

            T Output(T signal);
    };

    template<typename T>
    Filter<T>::Filter(int filter, int order, T cutoff, T resonance):
        _filter(filter),
        _order(order), 
        _cutoff(cutoff), 
        _resonance(resonance) {
        this->_feedback = this->GetFeedback();
        this->_buffer.resize(order);
        std::fill(this->_buffer.begin(), this->_buffer.end(), (T) 0.0);
    }

    template<typename T>
    void Filter<T>::SetOrder(int order) {
        long unsigned int old_size = this->_order;
        long unsigned int new_size = order;
        this->_order = order;
        this->_buffer.resize(new_size);

        if(new_size > old_size)
            std::fill(
                this->_buffer.begin() + old_size, 
                this->_buffer.end(), 
                (T) 0.0
            );
    }

    template<typename T>
    T Filter<T>::Output(T signal) {
        for(long unsigned int i = 0; i < this->_buffer.size(); i++) {
            if(i == 0)
                this->_buffer[i] += this->_cutoff * (
                    signal - this->_buffer[i] 
                    + this->_feedback * (this->_buffer[i] - this->_buffer[i + 1])
                );
            else
                this->_buffer[i] += this->_cutoff * (this->_buffer[i - 1] - this->_buffer[i]);
        }

        switch (this->_filter) {
            case Mode::LOW_PASS:
                return this->_buffer.back();

            case Mode::HIGH_PASS:
                return signal - this->_buffer.back();

            case Mode::BAND_PASS:
                return this->_buffer.front() - this->_buffer.back();
            
            default:
                return (T) 0.0;
        }
    }

}