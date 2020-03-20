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
        self.timbre.add_formant(wa.Oscillator.Style.TRIANGLE, 0.50, 12)
        self.adsr = wa.ADSR(0.0, 0.2, 0.0, 0.0)
        self.volume = volume
        
        self.instrument = wa.Instrument(self.timbre, self.adsr, volume)
        
    def __call__(self, time: float) -> float:
        return self.instrument(time)

    def note_on(self, time: float, id: int) -> None:
        self.instrument.note_on(time, id)

    def note_off(self, time: float, id: int) -> None:
        self.instrument.note_off(time, id)


class Arpegiator:
    def __init__(self, notes: List[int], delay: float) -> None:
        self.notes = notes
        self.delay = delay

    def __call__(self) -> None:
        global clock, harmonica
        last_note = None
        for note in cycle(self.notes):
            if last_note is not None:
                harmonica.note_off(clock.time, last_note)
            harmonica.note_on(clock.time, note)
            last_note = note
            time.sleep(self.delay)


arpegiator = Arpegiator([69, 67, 72, 65], 0.125)

audio = wa.Audio()
clock = wa.Clock()
harmonica = Harmonica(0.1)


def process_callback(dt: float) -> float:
    global clok, oscillator
    return harmonica(clock(dt))


audio.attach_callback(process_callback)
audio.init()
audio.run()

arpegiator()

audio.stop()