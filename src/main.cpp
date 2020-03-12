#include <iostream>

#include "const.hpp"
#include "audio.hpp"
#include "clock.hpp"
#include "osc.hpp"
#include "timbre.hpp"
#include "adsr.hpp"
#include "instrument.hpp"

template<typename T>
class Harmonica: public wavedream::Instrument<T> {
    public:
        Harmonica(T volume): wavedream::Instrument<T>(volume) {
            this->_timbre = new wavedream::Timbre<T>();
            this->_timbre->AddFormant(wavedream::SIN,      (T) 1.00, 0);
            this->_timbre->AddFormant(wavedream::SAWTOOTH, (T) 0.50, +12);
            this->_timbre->AddFormant(wavedream::NOISE,    (T) 0.25, 0);

            this->_adsr = new wavedream::ADSR<T>(0.0, 1.0, 0.95, 0.1);        
        }
};

static wavedream::Audio<FTYPE> *audio;

static Harmonica<FTYPE>* harmonica = new Harmonica<FTYPE>(0.5);
static wavedream::Clock<FTYPE> *oclock = new wavedream::Clock<FTYPE>();;

static FTYPE Update(FTYPE dt) {
    oclock->Update(dt);
    FTYPE time = oclock->GetTime();
    return harmonica->Output(time);
}

int main(int argc, char **argv) {
    audio = wavedream::Audio<FTYPE>::GetInstance();
    audio->SetProcessCallback(Update);
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