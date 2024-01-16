# FormantFilter
This is a formant filter object I created for the teensy audio library. It has one input, and one output. It works best with a band-limited pulse wave (I just run the default pulse wave through a one-pole filter in my testing), but it accepts any input, and it's fun to experiment.

I created this for the purpose of learning about how the teensy audio library works, as well as creating a single object that can easily handle formant filtering. It is still under development, but it works decently well as it is. I plan to create other objects as well, those under development include:

VocalVibrato: A simple vibrato that increases in depth over an adjustable amount of time, mimicing operatic singing.

AudioEffectMouth: A sort of sweeping filter that would mimic a "w" or a "l" sound, such as in "Wah" or "Lah." this one is less developed than the other two, but it seems like a good step towards consonant synthesis on the teensy.
