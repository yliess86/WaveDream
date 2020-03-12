#pragma once

namespace wavedream {

    template<typename T>
    class ADSR {
        private:
            T _attack_time;
            T _decay_time;
            T _sustain;
            T _release_time;

            T Amplitude(T life);

        public:
            ADSR(T attack_time, T decay_time, T sustain, T release_time): 
                _attack_time(attack_time),
                _decay_time(decay_time),
                _sustain(sustain),
                _release_time(release_time) {}

            T Envelope(T time, T time_on, T time_off);
    };

    template<typename T>
    T ADSR<T>::Amplitude(T life) {
        if(life <= this->_attack_time)
            return (life / this->_attack_time);
        
        if(life > this->_attack_time && life <= (this->_attack_time + this->_decay_time))
            return ((life - this->_attack_time) / this->_decay_time) * (this->_sustain - 1.0) + 1.0;
        
        if(life > (this->_attack_time + this->_decay_time))
            return this->_sustain;
        
        return (T) 0.0;
    }

    template<typename T>
    T ADSR<T>::Envelope(T time, T time_on, T time_off) {
        T life = 0.0;
        T amplitude = 0.0;
        T release_amplitude = 0.0;

        if(time_on > time_off) {
            life = time - time_on;
            amplitude = this->Amplitude(life);
        } 
        else {
            life = time_off - time_on;
            release_amplitude = this->Amplitude(life);
            amplitude = ((time - time_off) / this->_release_time) * (0.0 - release_amplitude) + release_amplitude;
        }

        return (amplitude <= 0.01)? 0.0: amplitude;
    }

}