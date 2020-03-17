#pragma once

#include <map>
#include <vector>

#include "timbre.hpp"
#include "note.hpp"
#include "adsr.hpp"
#include "const.hpp"

namespace wavedream {

    template<typename T>
    class Instrument {
        protected:
            Timbre<T> *_timbre;
            ADSR<T> *_adsr;
            T _volume;
        
        private:
            std::map<int, std::vector<Note<T>>> _notes;

        public:
            Instrument(Timbre<T> *timbre, ADSR<T> *adsr, T volume):
                _timbre(timbre), _adsr(adsr), _volume(volume) {}

            Timbre<T>* GetTimbre(void) { return this->_timbre; }
            void SetTimbre(Timbre<T> *timbre) { this->_timbre = timbre; }

            ADSR<T>* GetADSR(void) { return this->_adsr; }
            void SetADSR(ADSR<T> *adsr) { this->_adsr = adsr; }

            T GetVolume(void) { return this->_volume; }
            void SetVolume(T volume) { this->_volume = volume; }

            void NoteOn(T time, int id);
            void NoteOff(T time, int id);

            virtual T Output(T time);
    };

    template<typename T>
    T Instrument<T>::Output(T time) {
        T timbre;
        T envelope;
        int to_clean;

        T signal = (T) 0.0;

        for (auto&& [note_id, note_vec]: this->_notes) {
            to_clean = 0;
            
            for(auto&& note: note_vec) {
                timbre = this->_timbre->Output(time, note_id);
                envelope = this->_adsr->Envelope(time, note.on, note.off);
                signal += timbre * envelope;
                if(envelope == 0) note.active = false;
                if(!note.active) to_clean++;
            }

            if(to_clean > 0) note_vec.erase(note_vec.begin(), note_vec.begin() + to_clean);
        }

        return this->_volume * signal;
    }

    template<typename T>
    void Instrument<T>::NoteOn(T time, int id) {
        Note<T> note = Note<T>(id, time);
        this->_notes[note.id].push_back(note);
    }

    template<typename T>
    void Instrument<T>::NoteOff(T time, int id) {
        for(auto&& note: this->_notes[id]) {
            if(note.active && note.off <= 0.0) {
                note.off = time;
                break;
            }
        }
    }

}