from itertools import cycle
from typing import List

import sys
sys.path.append("../lib/")

import wavedream as wa
import time


class Instrument:
    def __init__(self, timbre: wa.Timbre, adsr: wa.ADSR, volume: float) -> None:
        self.timbre = timbre
        self.adsr = adsr
        self.volume = volume
        self.instrument = wa.Instrument(timbre, adsr, volume)

    def __call__(self, time: float) -> float:
        return self.instrument(time)

    def note_on(self, time: float, id: int) -> None:
        self.instrument.note_on(time, id)

    def note_off(self, time: float, id: int) -> None:
        self.instrument.note_off(time, id)


class Bell(Instrument):
    def __init__(self, volume: float) -> None:
        timbre = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0)
        ])
        adsr = wa.ADSR(0.01, 0.2, 0.0, 0.0)
        super(Bell, self).__init__(timbre, adsr, volume)


class Bass(Instrument):
    def __init__(self, volume: float) -> None:
        timbre = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00,  0),
            wa.Formant(wa.Oscillator.Style.SIN, 0.50, 12),
        ])
        adsr = wa.ADSR(0.01, 0.2, 0.0, 0.0)
        super(Bass, self).__init__(timbre, adsr, volume)
        

class HitHat(Instrument):
    def __init__(self, volume: float, closed: bool = True) -> None:
        timbre = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.NOISE,    1.00, 0),
            wa.Formant(wa.Oscillator.Style.SAWTOOTH, 0.25, 0),
        ])
        adsr = wa.ADSR(0.0, 0.05 if closed else 0.2, 0.0, 0.0)
        super(HitHat, self).__init__(timbre, adsr, volume)


class Kick(Instrument):
    def __init__(self, volume: float) -> None:
        timbre = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.SIN,   1.000, 0),
            wa.Formant(wa.Oscillator.Style.NOISE, 0.005, 0),
        ])
        adsr = wa.ADSR(0.0, 0.4, 0.0, 0.0)
        super(Kick, self).__init__(timbre, adsr, volume)


audio = wa.Audio()
clock = wa.Clock(1.0)

bell = Bell(0.05)
bass = Bass(0.2)
closed_hithat = HitHat(0.05, True)
open_hithat = HitHat(0.05, False)
kick = Kick(0.6)

delay = wa.Delay(48000, 0.50, 0.50)
reverb = wa.Reverb(48000, 0.2, 0.96, 1.0)

arpeg_bell = wa.Arpegiator([
    74, 74, 74, 72, 72, 74, 76, 69, 72, 79, 79, 76, 76, 74, 74, 72
], bell.instrument, 3.0)
arpeg_bass = wa.Arpegiator([38, 36, 43, 40], bass.instrument, 0.75)

arpeg_closed_hihhat = wa.Arpegiator([41], closed_hithat.instrument, 6.0)
arpeg_open_hihhat = wa.Arpegiator([41], open_hithat.instrument, 1.5)
arpeg_kick = wa.Arpegiator([40], kick.instrument, 3.0)

melody_sound = lambda t: (
    delay(arpeg_bell(t)) + delay(arpeg_bass(t))
)
drum_sound = lambda t: (
    arpeg_closed_hihhat(t) + arpeg_open_hihhat(t) + arpeg_kick(t)
) 


def process_callback(dt: float) -> float:
    global clok, oscillator
    t = clock(dt)
    return 0.8 * melody_sound(t) + drum_sound(t)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(20)

audio.stop()