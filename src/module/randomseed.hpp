#pragma once

#include <cmath>
#include <vector>

#include "../instrument/note.hpp"
#include "../instrument/instrument.hpp"

namespace wavedream {

    template<typename T>
    class RandomSeed {
        private:
            int _mean;
            int _std;
            std::vector<int> _notes;
            
            Instrument<T> *_instrument;
            T _freq;
            T _period;
            
            int _last;
            int _played;
            int _seed;

            void Process(T time) {
                if(this->_played > 1)
                    this->_instrument->NoteOff(time, _last);
                int value = std::rand() % (this->_std * 2) + (this->_mean - this->_std);
                int idx = std::max(std::min(value, (int) this->_notes.size() - 1), 0); 
                int note = this->_notes[idx];
                this->_instrument->NoteOn(time, note);
                this->_last = note;
            }

        public:
            RandomSeed(int mean, int stddev, Instrument<T> *instrument, T freq): 
                _mean(mean), 
                _std(stddev), 
                _instrument(instrument), 
                _freq(freq), 
                _period((T) 1.0 / freq),
                _last(0),
                _played(0),
                _seed(std::rand() % INT32_MAX) { 
                    std::srand(this->_seed);
                    for(int i = 0; i < 127; i++)
                        this->_notes.push_back(i);
                }

            RandomSeed(std::vector<int> notes, int stddev, Instrument<T> *instrument, T freq): 
                _mean(int(notes.size() / 2)), 
                _std(stddev), 
                _notes(notes),
                _instrument(instrument), 
                _freq(freq), 
                _period((T) 1.0 / freq),
                _last(0),
                _played(0),
                _seed(std::rand() % INT32_MAX) { 
                    std::srand(this->_seed);
                }

            RandomSeed(std::vector<int> notes, int mean, int stddev, Instrument<T> *instrument, T freq): 
                _mean(mean), 
                _std(stddev), 
                _notes(notes),
                _instrument(instrument), 
                _freq(freq), 
                _period((T) 1.0 / freq),
                _last(0),
                _played(0),
                _seed(std::rand() % INT32_MAX) { 
                    std::srand(this->_seed);
                }

            Instrument<T>* GetInstrument(void) { return this->_instrument; }
            void SetInstrument(Instrument<T> *instrument) { this->_instrument = instrument; }

            int GetMean(void) { return this->_mean; }
            void SetMean(int mean) { this->_mean = mean; }

            int GetStd(void) { return this->_std; }
            void SetStd(int stddev) { this->_std = stddev; }

            int GetSeed(void) { return this->_seed; }
            void SetSeed(int seed) { this->_seed = seed; std::srand(this->_seed); }
            void Randomize(void) { this->_seed = std::rand() % INT32_MAX; std::srand(this->_seed); }

            T GetFreq(void) { return this->_freq; }
            void SetFreq(T freq) { this->_freq = freq; this->_period = (T) 1.0 / freq; }

            T Output(T time);
    };

    template<typename T>
    T RandomSeed<T>::Output(T time) {
        int n = (int) std::floor(time / this->_period);
        if(n > this->_played) {
            int n_played = n - this->_played;
            this->_played += n_played;
            for(int i = 0; i < n_played; i++) {
                this->Process(time);
            }
        }
        return this->_instrument->Output(time);
    }

}