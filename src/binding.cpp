#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "core/audio.hpp"
#include "core/clock.hpp"
#include "core/const.hpp"
#include "core/lfo.hpp"
#include "core/osc.hpp"
#include "core/scale.hpp"

#include "instrument/adsr.hpp"
#include "instrument/instrument.hpp"
#include "instrument/note.hpp"
#include "instrument/timbre.hpp"

#include "fx/allpassfilter.hpp"
#include "fx/delay.hpp"
#include "fx/distortion.hpp"
#include "fx/filter.hpp"
#include "fx/lowpasssinglepole.hpp"
#include "fx/reverb.hpp"

#include "module/arpegiator.hpp"
#include "module/randomseed.hpp"

using namespace wavedream;
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
PYBIND11_MODULE(wavedream, m) {
    std::vector<double> notes;
    notes.assign(std::begin(NOTES), std::end(NOTES));

    m.attr("NOTES") = notes;
    m.attr("A0")    = A0;
    m.attr("A1")    = A1;
    m.attr("A2")    = A2;
    m.attr("A3")    = A3;
    m.attr("A4")    = A4;
    m.attr("A5")    = A5;

    py::class_<Audio<double>, std::unique_ptr<Audio<double>, py::nodelete>>(m, "Audio")
        .def(py::init(&Audio<double>::GetInstance), py::return_value_policy::reference)
        .def("init",            &Audio<double>::Init,                  "Init audio device and outstream.")
        .def("run",             &Audio<double>::Run,                   "Run audio output thread")
        .def("stop",            &Audio<double>::Stop,                  "Stop audio device and outstream.")
        .def("attach_callback", &Audio<double>::AttachProcessCallback, "Set process callback.")
        .def_property_readonly("sr", &Audio<double>::GetSampleRate);

    py::class_<Clock<double>>(m, "Clock")
        .def(py::init<>())
        .def(py::init<double>(), py::arg("speed"))
        .def_property_readonly("time", &Clock<double>::GetTime)
        .def_property("speed", &Clock<double>::GetSpeed, &Clock<double>::SetSpeed)
        .def(
            "__call__", &Clock<double>::Update, "Update time from dt.",
            py::arg("dt")
        );
    
    py::class_<Note<double>>(m, "Note")
        .def(py::init<int, double>(), py::arg("id"), py::arg("on"))
        .def_readwrite("id",       &Note<double>::id)
        .def_readwrite("on",       &Note<double>::on)
        .def_readwrite("off",      &Note<double>::off)
        .def_readwrite("active",   &Note<double>::active);

    py::class_<Oscillator<double>> osc(m, "Oscillator");

    osc.def(py::init<int>())
        .def_property_readonly("osc", &Oscillator<double>::GetOsc)
        .def(
            "__call__", &Oscillator<double>::Output, "Oscillator compute voltage output.",
            py::arg("time"), py::arg("freq")
        );

    py::enum_<Oscillator<double>::Style>(osc, "Style")
        .value("SIN",      Oscillator<double>::Style::SIN)
        .value("SQUARE",   Oscillator<double>::Style::SQUARE)
        .value("TRIANGLE", Oscillator<double>::Style::TRIANGLE)
        .value("SAWTOOTH", Oscillator<double>::Style::SAWTOOTH)
        .value("NOISE",    Oscillator<double>::Style::NOISE)
        .export_values();

    py::class_<LFO<double>>(m, "LFO")
        .def(py::init<int, double, double>(), py::arg("osc"), py::arg("amount"), py::arg("rate"))
        .def_property("osc",    &LFO<double>::GetOsc,    &LFO<double>::SetOsc)
        .def_property("amount", &LFO<double>::GetAmount, &LFO<double>::SetAmount)
        .def_property("rate",   &LFO<double>::GetRate,   &LFO<double>::SetRate)
        .def(
            "__call__", &LFO<double>::Output, "LFO compute voltage output",
            py::arg("time")
        );

    py::class_<Formant<double>>(m, "Formant")
        .def(py::init<int, double, int>(), py::arg("osc"), py::arg("amplitude"), py::arg("note_id_shift"))
        .def_readwrite("osc",           &Formant<double>::osc)
        .def_readwrite("amplitude",     &Formant<double>::amplitude)
        .def_readwrite("note_id_shift", &Formant<double>::note_id_shift);
    
    py::class_<Timbre<double>>(m, "Timbre")
        .def(py::init<>())
        .def(py::init<std::vector<Formant<double>>>(), py::arg("formants"))
        .def_property_readonly("formants", &Timbre<double>::GetFormant)
        .def(
            "add_formant", &Timbre<double>::AddFormant, "Add formant to timbre.",
            py::arg("osc"), py::arg("amplitude"), py::arg("note_id_shift")
        )
        .def(
            "__call__", &Timbre<double>::Output, "Timbre compute voltage ouput.",
            py::arg("tile"), py::arg("note_id")
        );

    py::class_<ADSR<double>>(m, "ADSR")
        .def(py::init<double, double, double, double>(), py::arg("attack"), py::arg("delay"), py::arg("sustain"), py::arg("release"))
        .def_property("attack",  &ADSR<double>::GetAttack,  &ADSR<double>::SetAttack)
        .def_property("decay",   &ADSR<double>::GetDecay,   &ADSR<double>::SetDecay)
        .def_property("sustain", &ADSR<double>::GetSustain, &ADSR<double>::SetSustain)
        .def_property("release", &ADSR<double>::GetRelease, &ADSR<double>::SetRelease)
        .def(
            "__call__", &ADSR<double>::Envelope, "Get ADSR enveloppe.",
            py::arg("time"), py::arg("time_on"), py::arg("time_off")
        );

    py::class_<Instrument<double>>(m, "Instrument")
        .def(py::init<Timbre<double>*, ADSR<double>*, double>(), py::arg("timbre"), py::arg("adsr"), py::arg("volume"))
        .def_property("timbre", &Instrument<double>::GetTimbre, &Instrument<double>::SetTimbre)
        .def_property("adsr",   &Instrument<double>::GetADSR,   &Instrument<double>::SetADSR)
        .def_property("volume", &Instrument<double>::GetVolume, &Instrument<double>::SetVolume)
        .def(
            "__len__", &Instrument<double>::GetNotesLeft, "Notes left to play."
        )
        .def(
            "note_on", &Instrument<double>::NoteOn, "Register note on event.", 
            py::arg("time"), py::arg("id")
        )
        .def(
            "note_off", &Instrument<double>::NoteOff, "Register note off event.", 
            py::arg("time"), py::arg("id")
        )
        .def(
            "__call__", &Instrument<double>::Output, "Instrument compute voltage output.", 
            py::arg("time")
        );

    py::class_<Delay<double>>(m, "Delay")
        .def(py::init<int, double, double>(), py::arg("sr"), py::arg("delay"), py::arg("feedback"))
        .def_property("delay",    &Delay<double>::GetDelay,    &Delay<double>::SetDelay)
        .def_property("feedback", &Delay<double>::GetFeedback, &Delay<double>::SetFeedback)
        .def(
            "__call__", &Delay<double>::Output, "Delayed signal.",
            py::arg("signal")
        );

    py::class_<AllPassFilter<double>>(m, "AllPassFilter")
        .def(py::init<int, double, double>(), py::arg("sr"), py::arg("delay"), py::arg("feedback"))
        .def_property("delay",    &AllPassFilter<double>::GetDelay,    &AllPassFilter<double>::SetDelay)
        .def_property("feedback", &AllPassFilter<double>::GetFeedback, &AllPassFilter<double>::SetFeedback)
        .def(
            "__call__", &AllPassFilter<double>::Output, "All passed filtered signal.",
            py::arg("signal")
        );

    py::class_<LowPassSinglePole<double>>(m, "LowPassSinglePole")
        .def(py::init<double>(), py::arg("damp"))
        .def_property("damp", &LowPassSinglePole<double>::GetDamp, &LowPassSinglePole<double>::SetDamp)
        .def(
            "__call__", &LowPassSinglePole<double>::Output, "Low passed single pole signal.",
            py::arg("signal")
        );

    py::class_<Reverb<double>>(m, "Reverb")
        .def(py::init<int, double, double, double>(), py::arg("sr"), py::arg("gain"), py::arg("feedback"), py::arg("wet"))
        .def(py::init<int, double, double>(), py::arg("sr"), py::arg("gain"), py::arg("wet"))
        .def_property("gain",     &Reverb<double>::GetGain,     &Reverb<double>::SetGain)
        .def_property("feedback", &Reverb<double>::GetFeedback, &Reverb<double>::SetFeedback)
        .def_property("wet",      &Reverb<double>::GetWet,      &Reverb<double>::SetWet)
        .def(
            "__call__", &Reverb<double>::Output, "Reverbed signal.",
            py::arg("signal")
        );

    py::class_<Distortion<double>>(m, "Distortion")
        .def(py::init<double, double>(), py::arg("drive"), py::arg("mix"))
        .def_property("drive", &Distortion<double>::GetDrive, &Distortion<double>::SetDrive)
        .def_property("mix",   &Distortion<double>::GetMix,   &Distortion<double>::SetMix)
        .def(
            "__call__", &Distortion<double>::Output, "Distorted signal.",
            py::arg("signal")
        );

    py::class_<Filter<double>> filter(m, "Filter");

    filter.def(py::init<int, int, double, double>(), py::arg("filter"), py::arg("order"), py::arg("cutoff"), py::arg("resonance"))
        .def_property("filter",    &Filter<double>::GetFilter,    &Filter<double>::SetFilter)
        .def_property("order",     &Filter<double>::GetOrder,     &Filter<double>::SetOrder)
        .def_property("cutoff",    &Filter<double>::GetCutOff,    &Filter<double>::SetCutoff)
        .def_property("resonance", &Filter<double>::GetResonance, &Filter<double>::SetResonance)
        .def_property_readonly("feedback", &Filter<double>::GetFeedback)
        .def(
            "__call__", &Filter<double>::Output, "Filtered output.",
            py::arg("signal")
        );

    py::enum_<Filter<double>::Mode>(filter, "Mode")
        .value("LOW_PASS",  Filter<double>::Mode::LOW_PASS)
        .value("HIGH_PASS", Filter<double>::Mode::HIGH_PASS)
        .value("BAND_PASS", Filter<double>::Mode::BAND_PASS)
        .export_values();

    py::class_<Arpegiator<double>>(m, "Arpegiator")
        .def(py::init<std::vector<int>, Instrument<double>*, double>(), py::arg("notes"), py::arg("instrument"), py::arg("freq"))
        .def_property("notes",      &Arpegiator<double>::GetNotes,      &Arpegiator<double>::SetNotes)
        .def_property("instrument", &Arpegiator<double>::GetInstrument, &Arpegiator<double>::SetInstrument)
        .def_property("freq",       &Arpegiator<double>::GetFreq,       &Arpegiator<double>::SetFreq)
        .def(
            "__call__", &Arpegiator<double>::Output, "Arpegiator output voltage.",
            py::arg("time")
        );

    py::class_<RandomSeed<double>>(m, "RandomSeed")
        .def(py::init<int, int, Instrument<double>*, double>(), py::arg("mean"), py::arg("std"), py::arg("instrument"), py::arg("freq"))
        .def_property("mean",       &RandomSeed<double>::GetMean,       &RandomSeed<double>::SetMean)
        .def_property("std",        &RandomSeed<double>::GetStd,        &RandomSeed<double>::SetStd)
        .def_property("seed",       &RandomSeed<double>::GetSeed,       &RandomSeed<double>::SetSeed)
        .def_property("instrument", &RandomSeed<double>::GetInstrument, &RandomSeed<double>::SetInstrument)
        .def_property("freq",       &RandomSeed<double>::GetFreq,       &RandomSeed<double>::SetFreq)
        .def("randomize", &RandomSeed<double>::Randomize, "Set a new random seed.")
        .def(
            "__call__", &RandomSeed<double>::Output, "RandomSeed output voltage.",
            py::arg("time")
        );

    py::class_<Scale>(m, "Scale")
        .def_static(
            "major", &Scale::Major, "Major scale", py::arg("key")
        )
        .def_static(
            "minor", &Scale::Minor, "Minor scale", py::arg("key")
        );
}