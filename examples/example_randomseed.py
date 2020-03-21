from itertools import cycle
from typing import List

import sys
sys.path.append("../lib/")

import wavedream as wa
import time


class Harmonica:
    def __init__(self, volume: float) -> None:
        self.timbre = wa.Timbre()
        self.timbre.add_formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0)
        self.timbre.add_formant(wa.Oscillator.Style.SQUARE, 0.50, 12)
        self.adsr = wa.ADSR(0.01, 0.2, 0.0, 0.0)
        self.volume = volume
        
        self.instrument = wa.Instrument(self.timbre, self.adsr, volume)
        
    def __call__(self, time: float) -> float:
        return self.instrument(time)

    def note_on(self, time: float, id: int) -> None:
        self.instrument.note_on(time, id)

    def note_off(self, time: float, id: int) -> None:
        self.instrument.note_off(time, id)


class HitHat:
    def __init__(self, volume: float) -> None:
        self.timbre = wa.Timbre()
        self.timbre.add_formant(wa.Oscillator.Style.NOISE,    1.00, 0)
        self.timbre.add_formant(wa.Oscillator.Style.SAWTOOTH, 0.25, 0)
        self.adsr = wa.ADSR(0.0, 0.1, 0.0, 0.0)
        self.volume = volume
        
        self.instrument = wa.Instrument(self.timbre, self.adsr, volume)
        
    def __call__(self, time: float) -> float:
        return self.instrument(time)

    def note_on(self, time: float, id: int) -> None:
        self.instrument.note_on(time, id)

    def note_off(self, time: float, id: int) -> None:
        self.instrument.note_off(time, id)


audio = wa.Audio()
clock = wa.Clock()

harmonica = Harmonica(0.05)
arpegiator_a = wa.RandomSeed(69, 12, harmonica.instrument, 6.0)
arpegiator_b = wa.RandomSeed(69, 12, harmonica.instrument, 2.0)

hithat = HitHat(0.05)
arpegiator_c = wa.Arpegiator([41], hithat.instrument, 6.0)

delay = wa.Delay(48000, 0.50, 0.50)
reverb = wa.Reverb(48000, 0.2, 0.96, 1.0)


def process_callback(dt: float) -> float:
    global clok, oscillator
    t = clock(dt)
    return delay(arpegiator_a(t)) + reverb(arpegiator_b(t)) + arpegiator_c(t)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(20)

audio.stop()