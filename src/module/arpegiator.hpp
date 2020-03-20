#pragma once

#include <vector>
#include <cmath>

#include "../instrument/note.hpp"
#include "../instrument/instrument.hpp"

namespace wavedream {

    template<typename T>
    class Arpegiator {
        private:
            std::vector<int> _notes;
            Instrument<T> *_instrument;
            T _freq;
            T _period;
            
            long unsigned int _idx;
            long unsigned int _last_idx;
            int _played;

            void Increment(void) {
                this->_idx++;
                if(this->_played > 1) this->_last_idx++;
                if(this->_idx >= this->_notes.size()) this->_idx = 0;
                if(this->_last_idx >= this->_notes.size()) this->_last_idx = 0;
            }
            void Process(T time) {
                if(this->_played > 1)
                    this->_instrument->NoteOff(time, this->_notes[this->_last_idx]);
                this->_instrument->NoteOn(time, this->_notes[this->_idx]);
            }

        public:
            Arpegiator(std::vector<int> notes, Instrument<T> *instrument, T freq): 
                _notes(notes), 
                _instrument(instrument), 
                _freq(freq), 
                _period((T) 1.0 / freq), 
                _idx(0), 
                _last_idx(0),
                _played(0) {}

            std::vector<int> GetNotes(void) { return this->_notes; }
            void SetNotes(std::vector<int> notes) { this->_notes = notes; }

            Instrument<T>* GetInstrument(void) { return this->_instrument; }
            void SetInstrument(Instrument<T> *instrument) { this->_instrument = instrument; }

            T GetFreq(void) { return this->_freq; }
            void SetFreq(T freq) { this->_freq = freq; this->_period = (T) 1.0 / freq; }

            T Output(T time);
    };

    template<typename T>
    T Arpegiator<T>::Output(T time) {
        int n = (int) std::floor(time / this->_period);
        if(n > this->_played) {
            int n_played = n - this->_played;
            this->_played += n_played;
            for(int i = 0; i < n_played; i++) {
                this->Process(time);
                this->Increment();
            }
        }
        return this->_instrument->Output(time);
    }

}