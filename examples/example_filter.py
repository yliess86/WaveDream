import sys
sys.path.append("../lib/")

import wavedream as wa
import time


class Harmonica:
    def __init__(self, volume: float) -> None:
        self.timbre = wa.Timbre()
        self.timbre.add_formant(wa.Oscillator.Style.SIN,   1.00,  0)
        self.timbre.add_formant(wa.Oscillator.Style.SIN,   0.50, 12)
        self.adsr = wa.ADSR(0.0, 1.0, 0.95, 0.1)
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
harmonica = Harmonica(0.5)
pass_filter = wa.Filter(wa.Filter.Mode.HIGH_PASS, order=2, cutoff=0.44, resonance=0.66)


def process_callback(dt: float) -> float:
    global clok, oscillator
    return pass_filter(harmonica(clock(dt)))


audio.attach_callback(process_callback)
audio.init()
audio.run()
time.sleep(0.1)

harmonica.note_on(clock.time, 69)
time.sleep(2)
harmonica.note_off(clock.time, 69)

pass_filter = lambda x: x
time.sleep(0.1)

harmonica.note_on(clock.time, 69)
time.sleep(2)
harmonica.note_off(clock.time, 69)

audio.stop()