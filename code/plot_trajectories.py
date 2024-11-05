import numpy as np
import matplotlib.pyplot as plt

DATA = np.loadtxt("example.txt")
time = np.loadtxt("time.txt")
bn_data = np.fromfile("example.bin",dtype=np.float64)

N_time = time.shape[0]

if bn_data.shape[0]%N_time != 0:
    print(f"Time and data shapes are not compatible: {bn_data.shape[0]}, {N_time}, {bn_data.shape[0]%N_time}")
    exit()
else:
    N_trajs = bn_data.shape[0]//N_time

fig,ax = plt.subplots()

for i in range(DATA.shape[0]):
    ax.plot(DATA[i,:])

fig_2,ax_2 = plt.subplots()
for i in range(N_trajs):
    ax_2.plot(bn_data[i*N_time:(i+1)*N_time])

plt.show()
