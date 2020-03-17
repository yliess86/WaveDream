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

static wa::Audio<FTYPE> *audio;
static wa::Clock<FTYPE> *oclock = new wa::Clock<FTYPE>();;

static wa::Timbre<FTYPE> *timbre = new wa::Timbre<FTYPE>();
static wa::ADSR<FTYPE> *adsr = new wa::ADSR<FTYPE>(0.0, 1.0, 0.95, 0.1);
static wa::Instrument<FTYPE> *harmonica = new wa::Instrument<FTYPE>(timbre, adsr, (FTYPE) 0.5);

static FTYPE Update(FTYPE dt) {
    oclock->Update(dt);
    FTYPE time = oclock->GetTime();
    return harmonica->Output(time);
}

int main(int argc, char **argv) {
    timbre->AddFormant(wa::Oscillator<FTYPE>::Style::SIN,      (FTYPE) 1.00,  0);
    timbre->AddFormant(wa::Oscillator<FTYPE>::Style::SAWTOOTH, (FTYPE) 0.50, 12);
    timbre->AddFormant(wa::Oscillator<FTYPE>::Style::NOISE,    (FTYPE) 0.25,  0);

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