#pragma once

#include <map>
#include <vector>
#include <mutex>

#include "timbre.hpp"
#include "note.hpp"
#include "adsr.hpp"
#include "../core/const.hpp"

namespace wavedream {

    template<typename T>
    class Instrument {
        protected:
            Timbre<T> *_timbre;
            ADSR<T> *_adsr;
            T _volume;
        
        private:
            std::map<int, std::vector<Note<T>>> _notes;
            std::mutex _lock;

        public:
            Instrument(Timbre<T> *timbre, ADSR<T> *adsr, T volume):
                _timbre(timbre), _adsr(adsr), _volume(volume) {}

            Timbre<T>* GetTimbre(void) { return this->_timbre; }
            void SetTimbre(Timbre<T> *timbre) { this->_timbre = timbre; }

            ADSR<T>* GetADSR(void) { return this->_adsr; }
            void SetADSR(ADSR<T> *adsr) { this->_adsr = adsr; }

            T GetVolume(void) { return this->_volume; }
            void SetVolume(T volume) { this->_volume = volume; }

            int GetNotesLeft(void) {
                int size = 0;
                this->_lock.lock();
                for (auto& [note_id, note_vec]: this->_notes)
                    size += note_vec.size();
                this->_lock.unlock();
                return size;
            }
            
            void NoteOn(T time, int id);
            void NoteOff(T time, int id);

            virtual T Output(T time);
    };

    template<typename T>
    T Instrument<T>::Output(T time) {
        T timbre;
        T envelope;
        T signal = (T) 0.0;

        this->_lock.lock();
        for(auto map_it: this->_notes) {
            int id = map_it.first;
            std::vector<Note<T>> notes = map_it.second;

            auto note = notes.begin();
            while(note != notes.end()) {
                timbre = this->_timbre->Output(time, id);
                envelope = this->_adsr->Envelope(time, note->on, note->off);
                signal += timbre * envelope;
                
                if(envelope <= 0.0 && note->off > 0.0) {
                    note->active = false;
                    note = notes.erase(note);
                }
                else ++note;
            }
        }
        this->_lock.unlock();

        return this->_volume * signal;
    }

    template<typename T>
    void Instrument<T>::NoteOn(T time, int id) {
        Note<T> note = Note<T>(id, time);
        this->_lock.lock();
        this->_notes[note.id].push_back(note);
        this->_lock.unlock();
    }

    template<typename T>
    void Instrument<T>::NoteOff(T time, int id) {
        this->_lock.lock();
        for(auto note: this->_notes[id]) {
            if(note.active && note.off <= 0.0) {
                note.off = time;
                break;
            }
        }
        this->_lock.unlock();
    }

}