import sys
sys.path.append("lib/")

import wavedream as wa
import time


class HitHat(wa.Instrument):
    def __init__(self, volume: float, closed: bool = True) -> None:
        self.t = wa.Timbre([
            wa.Formant(wa.Oscillator.Style.NOISE,    1.00, 0),
            wa.Formant(wa.Oscillator.Style.SAWTOOTH, 0.25, 0),
        ])
        self.a = wa.ADSR(0.01, 0.02 if closed else 0.2, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


class Kick(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([wa.Formant(wa.Oscillator.Style.SIN, 1.000, 0)])
        self.a = wa.ADSR(0.01, 0.5, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


class DrumMachine:
    def __init__(self, volume: float) -> None:
        self.volume = volume

        self.ch = HitHat(0.2, True)
        self.oh = HitHat(0.2, False)
        self.k  = Kick(0.6)

        self.closed_hihat_1 = wa.Arpegiator([44], self.ch,  6.00)
        self.closed_hihat_2 = wa.Arpegiator([44], self.ch,  1.80)
        self.open_hihat     = wa.Arpegiator([44], self.oh,  1.50)
        self.kick           = wa.Arpegiator([44], self.k,   3.00)

    def __call__(self, time: float) -> float:
        return self.volume * (0.0
            + self.closed_hihat_1(time) + self.closed_hihat_2(time)
            + self.open_hihat(time)
            + self.kick(time)
        )


class Clouds(wa.Instrument):
    def __init__(self, volume: float) -> None:
        self.t = wa.Timbre([wa.Formant(wa.Oscillator.Style.SIN, 1.00, 0)])
        self.a = wa.ADSR(4.0, 4.0, 0.0, 0.0)
        wa.Instrument.__init__(self, self.t, self.a, volume)


audio = wa.Audio()
clock = wa.Clock(1.0)

drum_machine  = DrumMachine(1.0)


def process_callback(dt: float) -> float:
    global clok, oscillator
    t = clock(dt)
    return drum_machine(t)


audio.attach_callback(process_callback)
audio.init()
audio.run()

time.sleep(60)

audio.stop()