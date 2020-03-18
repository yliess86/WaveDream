#pragma once

namespace wavedream {
    
    template<typename T>
    class LowPassSinglePole {
        private:
            T _damp;
            T _y;

        public:
            LowPassSinglePole(T damp): _damp(damp), _y((T) 0.0) {}

            T GetDamp(void) { return this->_damp; }
            void SetDamp(T damp) { this->_damp = damp; }

            T Output(T signal);
    };

    template<typename T>
    T LowPassSinglePole<T>::Output(T signal) {
        this->_y += ((T) 1.0 - this->_damp) * (signal - this->_y);
        return this->_y;
    }

}