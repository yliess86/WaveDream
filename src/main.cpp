#include <iostream>
#include <functional>

#include "core/const.hpp"
#include "core/audio.hpp"
#include "core/clock.hpp"
#include "core/osc.hpp"
#include "core/timbre.hpp"
#include "core/adsr.hpp"
#include "core/instrument.hpp"
namespace wa = wavedream;

template<typename T>
class Harmonica: public wa::Instrument<T> {
    public:
        Harmonica(T volume): wa::Instrument<T>(volume) {
            this->_timbre = new wa::Timbre<T>();
            this->_timbre->AddFormant(wa::Oscillator<T>::Style::SIN,      (T) 1.00, 0);
            this->_timbre->AddFormant(wa::Oscillator<T>::Style::SAWTOOTH, (T) 0.50, +12);
            this->_timbre->AddFormant(wa::Oscillator<T>::Style::NOISE,    (T) 0.25, 0);

            this->_adsr = new wa::ADSR<T>(0.0, 1.0, 0.95, 0.1);        
        }
};

static wa::Audio<FTYPE> *audio;

static Harmonica<FTYPE>* harmonica = new Harmonica<FTYPE>(0.5);
static wa::Clock<FTYPE> *oclock = new wa::Clock<FTYPE>();;

static FTYPE Update(FTYPE dt) {
    oclock->Update(dt);
    FTYPE time = oclock->GetTime();
    return harmonica->Output(time);
}

int main(int argc, char **argv) {
    std::function<FTYPE(FTYPE)> callback = Update; 

    audio = wa::Audio<FTYPE>::GetInstance();
    audio->AttachProcessCallback(callback);
    audio->Init();
    audio->Run();

    harmonica->NoteOn(0.1, 69);
    bool off = false;
    while(true) {
        if(oclock->GetTime() > 5.0 && !off) {
            harmonica->NoteOff(oclock->GetTime(), 69);
            off = true;
        }
    }

    audio->Stop();

    return 0;
}