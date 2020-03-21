from itertools import cycle
from typing import List

import sys
sys.path.append("../lib/")

import wavedream as wa
import time


class Bell(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0)
        ])
        self.a = wa.ADSR(0.01, 0.2, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


class Bass(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0),
            wa.Formant(wa.Oscillator.Style.SIN, 0.50, 12),
        ])
        self.a = wa.ADSR(0.01, 0.2, 0.8, 0.1)
        wa.Instrument.__init__(self, self.t, self.a, volume)
        

class HitHat(wa.Instrument):
    def __init__(self, volume: float, closed: bool = True) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.NOISE,    1.00, 0),
            wa.Formant(wa.Oscillator.Style.SAWTOOTH, 0.25, 0),
        ])
        self.a = wa.ADSR(0.01, 0.05 if closed else 0.2, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


class Kick(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.SIN,   1.000, 0),
            wa.Formant(wa.Oscillator.Style.NOISE, 0.005, 0),
        ])
        self.a = wa.ADSR(0.01, 0.4, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock(1.0)

bell          = Bell(0.05)
bass          = Bass(0.2)
closed_hithat = HitHat(0.05, True)
open_hithat   = HitHat(0.05, False)
kick          = Kick(0.6)

delay  = wa.Delay(48000, 0.50, 0.50)
reverb = wa.Reverb(48000, 0.2, 0.96, 1.0)

notes = [74, 74, 74, 72, 72, 74, 76, 69, 72, 79, 79, 76, 76, 74, 74, 72]
arpeg_bell          = wa.Arpegiator(notes,            bell,          3.00)
arpeg_bass          = wa.Arpegiator([38, 36, 43, 40], bass,          0.75)
arpeg_closed_hihhat = wa.Arpegiator([41],             closed_hithat, 6.00)
arpeg_open_hihhat   = wa.Arpegiator([41],             open_hithat,   1.50)
arpeg_kick          = wa.Arpegiator([40],             kick,          3.00)

melody_sound = lambda t: delay(arpeg_bell(t))   + delay(arpeg_bass(t))
hihat_sound  = lambda t: arpeg_closed_hihhat(t) + arpeg_open_hihhat(t) 
drum_sound   = lambda t: hihat_sound(t)         + arpeg_kick(t)


def process_callback(dt: float) -> float:
    global clok, oscillator
    t = clock(dt)
    return 0.8 * melody_sound(t) + drum_sound(t)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(20)

audio.stop()