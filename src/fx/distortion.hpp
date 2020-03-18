#pragma once

namespace wavedream {

    template<typename T>
    class Distortion {
        private:
            T _drive;
            T _mix;

        public:
            Distortion(T drive, T mix): _drive(drive), _mix(mix) {}

            T GetDrive(void) { return this->_drive; }
            void SetDrive(T drive) { this->_drive = drive; }

            T GetMix(void) { return this->_mix; }
            void SetMix(T mix) { this->_mix = mix; }

            T Output(T signal);
    };

    template<typename T>
    T Distortion<T>::Output(T signal) {
        signal = signal * this->_drive;
        T sq_signal = signal * signal;
        T cu_signal = sq_signal * signal;
        T out = signal - ((T) 0.15 * sq_signal) - ((T) 0.15 * cu_signal);
        return ((T) 1.0 - this->_mix) * signal + this->_mix * out;
    }

}