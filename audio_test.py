# This code can be used to plot and listen to the samples recorded by the microphone
# Just uncomment the line on the arduino code and copy on samples the results
# The code will plot the sampled audio and store it as a .wav file


import matplotlib.pyplot as plt
samples = ['''2398, 4024, 3493, 3770, 3582, 3702, 3671, 3665, ....'''] # Copy here your samples
print(len(samples))

import numpy as np
from scipy.io.wavfile import write

rate = 16000
scaled = np.int16(samples / np.max(np.abs(samples)) * 32767)
write('test.wav', rate, scaled)

plt.figure()
plt.plot(samples)
plt.show()