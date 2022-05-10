import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

data_path = "/home/deb/Documents/NetworkViewport/build/examples/data/"

x_traj_csv = np.genfromtxt(data_path + "x_traj.csv", delimiter=", ")

params = pd.read_csv(data_path + "param.csv")
print(params.columns)

x_trajs = np.split(x_traj_csv, params['count'][0], axis=0)

t = np.genfromtxt(data_path + "tvec.csv")

Nt = params['Nt'][0]
print(len(x_trajs))
for x_traj in x_trajs:
    if (np.all(x_traj == x_trajs[-1])):
        plt.plot(t[1:], x_traj[:,0], color='b', label='Susceptible')
        plt.plot(t[1:], x_traj[:,1], color='r', label='Infected')
        plt.plot(t[1:], x_traj[:,2], color='g', label='Recovered')
    else:
        plt.plot(t[1:], x_traj[:,0], color='b')
        plt.plot(t[1:], x_traj[:,1], color='r')
        plt.plot(t[1:], x_traj[:,2], color='g')
plt.legend()
plt.show()





