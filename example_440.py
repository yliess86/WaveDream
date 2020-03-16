import sys
sys.path.append("./lib/")

import wavedream as wa
import time

audio = wa.Audio()
clock = wa.Clock()
oscillator = wa.Oscillator(wa.Oscillator.Style.SIN)

def process_callback(dt: float) -> float:
    global clok, oscillator
    clock(dt)
    return oscillator(clock.time, 440)

audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(4)
aduio.stop()