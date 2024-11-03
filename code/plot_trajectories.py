import numpy as np
import matplotlib.pyplot as plt

DATA = np.loadtxt("example.txt")

fig,ax = plt.subplots()

for i in range(DATA.shape[0]):
    ax.plot(DATA[i,:])

plt.show()
