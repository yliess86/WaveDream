import wavedream as wa
import time


class Harmonica(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.SIN,   1.00,  0),
            wa.Formant(wa.Oscillator.Style.SIN,   0.50, 12),
        ])
        self.a = wa.ADSR(0.0, 1.0, 0.95, 0.1)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock()

harmonica = Harmonica(0.5)
pass_filter = wa.Filter(
    wa.Filter.Mode.HIGH_PASS, 
    order=2, 
    cutoff=0.33, 
    resonance=0.77
)


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