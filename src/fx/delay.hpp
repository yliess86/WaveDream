#pragma once

#include <vector>

namespace wavedream {

    template<typename T>
    class Delay {
        private:
            int _sr;
            T _delay;
            T _feedback;
            std::vector<T> _samples;
            long unsigned int _sample_idx;

        public:
            Delay(int sr, T delay, T feedback);

            T GetDelay(void) { return this->_delay; }
            void SetDelay(T delay);
            
            T GetFeedback(void) { return this->_feedback; }
            void SetFeedback(T feedback) { this->_feedback = feedback; }

            T Output(T signal);
    };

    template<typename T>
    Delay<T>::Delay(int sr, T delay, T feedback): 
        _sr(sr), _delay(delay), _feedback(feedback), _sample_idx(0) {
        this->_samples.resize((long unsigned int) (delay * sr));
        std::fill(this->_samples.begin(), this->_samples.end(), (T) 0.0);
    }

    template<typename T>
    void Delay<T>::SetDelay(T delay) { 
        this->_delay = delay;
        long unsigned int new_size = delay * this->_sr;
        long unsigned int old_size = this->_samples.size();
        this->_samples.resize(new_size);

        if(this->_sample_idx >= new_size)
            this->_sample_idx = 0;
        if(new_size > old_size)
            std::fill(
                this->_samples.begin() + old_size, 
                this->_samples.end(), 
                (T) 0.0
            );
    }

    template<typename T>
    T Delay<T>::Output(T signal) {
        T ret = signal + this->_samples[this->_sample_idx];
        this->_samples[this->_sample_idx] = this->_feedback * ret;
        
        this->_sample_idx++;
        if(this->_sample_idx >= this->_samples.size())
            this->_sample_idx = 0; 
        
        return ret;
    }

}