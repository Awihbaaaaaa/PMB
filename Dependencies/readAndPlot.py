# This file includes data extraction from the rosbag file and plotting the data
# in a polar coordinates

import subprocess
import matplotlib.pyplot as plt
import pandas as pd
import ast
from plot_circle_slice import plot_circle_slice

# Plot a circle slice with radius 100 and a field of view of 60 degrees
fig, ax = plot_circle_slice(100, 60)  

# Compile and run the C++ program to generate data points and read them from the console
""" 
cpp_file = "main.cpp"
exe_file = "main"

subprocess.run(["g++", cpp_file, "-o", exe_file])
data = subprocess.Popen(["./" + exe_file], stdout=subprocess.PIPE, universal_newlines=True)
"""
"""
Read the csv file to extract the data
"""
data_cloud = pd.read_csv('ros_data.csv')


# Keep the figure iteratively open
plt.ion()

scatter = ax.scatter([], [], c='red', label="Data Point")

# Plot the data
for ind, t in data_cloud.iloc[1:].iterrows():
    """
    The following is to extract the data points from a csv file. The csv file
    indludes the data as strings. The strings should be casted to floats. x & y are floats
    to be plotted
    """
    x = [float(item) for item in ast.literal_eval(data_cloud.x[ind])]
    y = [-float(item) for item in ast.literal_eval(data_cloud.y[ind])]
    vel = [float(item) for item in ast.literal_eval(data_cloud.vel[ind])]

    # Empty the scatter
    scatter.remove()
    scatter = ax.scatter(y, x, s=2.0, c='red', label="Data Point")
    plt.draw()
    plt.pause(0.8)
    
plt.ioff()
plt.show()



