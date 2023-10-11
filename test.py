import subprocess
import matplotlib.pyplot as plt
import time
#import numpy as np

from plot_circle_slice import plot_circle_slice
""" 
def plot_circle_slice(r, theta):
    if r < 0:
        raise ValueError("Radius (r) must be non-negative.")
    
    if theta < 0 or theta > 360:
        raise ValueError("Angle (theta) must be between 0 and 360 degrees.")

    # Generate theta values between 0 and the given angle in radians
    theta_rad = np.linspace(0, np.deg2rad(theta), 100)

    # Calculate x and y coordinates for the circular slice
    y = r * np.cos(theta_rad)
    x = r * np.sin(theta_rad)

    # Make subplots returned to 
    fig, ax = plt.subplots(figsize=(6,6))
    ax.plot(x, y, color='blue')
    ax.plot(-x, y, color='blue')
    ax.set_xlim(-r, r)
    ax.set_ylim(-10, r + 10)
    ax.set_aspect('equal', adjustable='box')

    ax.plot([0, x[-1]], [0, y[-1]], linestyle='-', color='blue', label='Radial Line')
    ax.plot([0, -x[-1]], [0, y[-1]], linestyle='-', color='blue', label='Radial Line')
    
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_title(f'Circle Slice (r={r}, theta={theta} degrees)')
    ax.grid(True)
    
    return fig, ax """

# Example usage:
fig, ax = plot_circle_slice(100, 50)  # Plot a circle slice with radius 5 and 60 degrees angle


# Compile and run the C++ program to generate data
cpp_file = "main.cpp"
exe_file = "main"

subprocess.run(["g++", cpp_file, "-o", exe_file])
data = subprocess.Popen(["./" + exe_file], stdout=subprocess.PIPE, universal_newlines=True)
#x_data, y_data = zip(*(line.split() for line in data))

# Convert data to floats
x_data = []
y_data = []


# Keep the figure iteratively open
plt.ion()

scatter = ax.scatter([], [], c='red', label="Data Point")

# Plot the data
for t in data.stdout:
    x, y = map(float, t.strip().split())

    scatter.remove()
    scatter = ax.scatter(x,y, c='red', label="Data Point")
    plt.draw()
    plt.pause(0.1)
    
plt.ioff()
plt.show()



