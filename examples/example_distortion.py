import wavedream as wa
import time


class Harmonica(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.SIN,   1.00,  0),
            wa.Formant(wa.Oscillator.Style.SIN,   0.50, 12),
            wa.Formant(wa.Oscillator.Style.NOISE, 0.05,  0),
        ])
        self.a = wa.ADSR(0.0, 1.0, 0.95, 0.1)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock()

harmonica = Harmonica(0.5)
distortion = wa.Distortion(50.0, 1.0)


def process_callback(dt: float) -> float:
    global clok, oscillator
    return 0.2 * distortion(harmonica(clock(dt)))


notes = [
    (69, 1.00), (72, 0.50), (67, 1.00),
    (65, 0.50), (67, 0.50), (69, 1.00),
    (72, 0.50), (67, 2.00),
    (69, 1.00), (72, 1.00), (79, 0.50),
    (77, 0.50), (72, 1.00),
    (70, 0.50), (69, 0.50), (67, 1.00), 
]
notes_iter = zip(notes + [(None, None)], [(None, None)] + notes)

audio.attach_callback(process_callback)
audio.init()
audio.run()
time.sleep(0.1)

for i, ((note, sleep), (p_note, _)) in enumerate(notes_iter):
    if p_note is not None:
        harmonica.note_off(clock.time, p_note)
    if note is not None:
        harmonica.note_on(clock.time, note)
        time.sleep(sleep)

time.sleep(5.0)
audio.stop()