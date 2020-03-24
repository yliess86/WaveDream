from itertools import cycle
from typing import List

import wavedream as wa
import time


class Bell(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([wa.Formant(wa.Oscillator.Style.TRIANGLE, 1.00, 0)])
        self.a = wa.ADSR(0.01, 0.4, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock()

bell_1 = Bell(0.04)
bell_2 = Bell(0.04)
bell_3 = Bell(0.04)

major = wa.Scale.major(69)

arpegiator_1 = wa.Arpegiator(major[46:46 + 7][::+1], bell_1, 16.0)
arpegiator_2 = wa.Arpegiator(major[32:32 + 7][::+2], bell_2,  4.0)
arpegiator_3 = wa.Arpegiator(major[25:25 + 7][::-1], bell_3,  2.0)
arpegiator   = lambda t: 2.0 * (0.0
    + 0.2 * arpegiator_1(t) 
    + 0.8 * arpegiator_2(t) 
    + 1.5 * arpegiator_3(t)
)


def process_callback(dt: float) -> float:
    global clok, arpegiator
    return arpegiator(clock(dt))
    

audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(6.2)

audio.stop()
