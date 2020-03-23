import wavedream as wa
import time


audio = wa.Audio()
clock = wa.Clock()
oscillator = wa.Oscillator(wa.Oscillator.Style.SIN)

phase_lfo = wa.LFO(wa.Oscillator.Style.SIN, 0.001, 5.0)
amplitude_lfo = wa.LFO(wa.Oscillator.Style.SIN, 0.5, 0.5)


def process_callback(dt: float) -> float:
    global clok, oscillator, lfo
    t = clock(dt)
    return amplitude_lfo(t) * oscillator(t + phase_lfo(t), wa.A4)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(4)

audio.stop()