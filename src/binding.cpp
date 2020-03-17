#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "core/adsr.hpp"
#include "core/audio.hpp"
#include "core/clock.hpp"
#include "core/const.hpp"
#include "core/instrument.hpp"
#include "core/note.hpp"
#include "core/osc.hpp"
#include "core/timbre.hpp"
using namespace wavedream;

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
        .def("attach_callback", &Audio<double>::AttachProcessCallback, "Set process callback.");

    py::class_<Clock<double>>(m, "Clock")
        .def(py::init<>())
        .def_property_readonly("time", &Clock<double>::GetTime)
        .def_property("speed", &Clock<double>::GetSpeed, &Clock<double>::SetSpeed)
        .def(
            "__call__", &Clock<double>::Update, "Update time from dt.",
            py::arg("dt")
        );
    
    py::class_<Note<double>>(m, "Note")
        .def(py::init<int, double>())
        .def_readwrite("id",     &Note<double>::id)
        .def_readwrite("on",     &Note<double>::on)
        .def_readwrite("off",    &Note<double>::off)
        .def_readwrite("active", &Note<double>::active);

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

    py::class_<Formant<double>>(m, "Formant")
        .def(py::init<Oscillator<double>*, double, int>())
        .def_readwrite("osc",           &Formant<double>::osc)
        .def_readwrite("amplitude",     &Formant<double>::amplitude)
        .def_readwrite("note_id_shift", &Formant<double>::note_id_shift);
    
    py::class_<Timbre<double>>(m, "Timbre")
        .def(py::init<>())
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
        .def(py::init<double, double, double, double>())
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
}