from itertools import cycle
from typing import List

import wavedream as wa
import time


class Harmonica(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0),
            wa.Formant(wa.Oscillator.Style.SQUARE, 0.50, 12),
        ])
        self.a = wa.ADSR(0.01, 0.2, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


class HitHat(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.NOISE,    1.00, 0),
            wa.Formant(wa.Oscillator.Style.SAWTOOTH, 0.25, 0),
        ])
        self.a = wa.ADSR(0.0, 0.1, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock()

harmonica = Harmonica(0.05)
hithat    = HitHat(0.05)

delay  = wa.Delay(48000, 0.50, 0.50)
reverb = wa.Reverb(48000, 0.2, 0.96, 1.0)

notes = [65, 67, 69, 72, 69, 67, 65, 67, 69, 74, 69, 67]
arpegiator_a = wa.Arpegiator(notes,            harmonica, 6.0)
arpegiator_b = wa.Arpegiator([41, 48, 41, 50], harmonica, 2.0)
arpegiator_c = wa.Arpegiator([41],             hithat,    6.0)


def process_callback(dt: float) -> float:
    global clok, oscillator
    t = clock(dt)
    return delay(arpegiator_a(t)) + reverb(arpegiator_b(t)) + arpegiator_c(t)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(20)

audio.stop()