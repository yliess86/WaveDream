import wavedream as wa
import time


audio = wa.Audio()
clock = wa.Clock()
oscillator = wa.Oscillator(wa.Oscillator.Style.SIN)


def process_callback(dt: float) -> float:
    global clok, oscillator
    return oscillator(clock(dt), wa.A4)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(2)

audio.stop()