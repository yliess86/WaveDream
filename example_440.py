import sys
sys.path.append("./lib/")

import wavedream as wa
import time

audio = wa.Audio()
clock = wa.Clock()
oscillator = wa.Oscillator(wa.Oscillator.Style.SIN)

def process_callback(dt: float) -> float:
    global clok, oscillator
    time = clock(dt)
    return oscillator(time, wa.A4)

audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(4)
audio.stop()