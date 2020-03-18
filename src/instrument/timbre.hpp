#pragma once

#include <vector>

#include "../core/osc.hpp"
#include "../core/const.hpp"

namespace wavedream {

    template<typename T>
    struct Formant {
        Oscillator<T> *osc;
        T amplitude;
        int note_id_shift;

        Formant(Oscillator<T> *osc, T amplitude, int note_id_shift):
            osc(osc), amplitude(amplitude), note_id_shift(note_id_shift) {}
    };

    template<typename T>
    class Timbre {
        private:
            std::vector<Formant<T>> _formants;

        public:
            std::vector<Formant<T>> GetFormant(void) { return this->_formants; }

            void AddFormant(int osc, T amplitude, int note_id_shift);
            T Output(T time, int note_id);
    };

    template<typename T>
    void Timbre<T>::AddFormant(int osc, T amplitude, int note_id_shift) {
        Formant<T> formant = Formant<T>(
            new Oscillator<T>(osc), amplitude, note_id_shift
        );

        this->_formants.push_back(formant);
    }

    template<typename T>
    T Timbre<T>::Output(T time, int note_id) {
        T freq;
        T output;
        T signal = (T) 0.0;
        for(auto&& formant: this->_formants) {
            freq = NOTES[note_id + formant.note_id_shift];
            output = formant.osc->Output(time, freq);
            signal += formant.amplitude * output;
        }

        int size = this->_formants.size();
        return (size > 0)? signal / size: signal;
    }

}