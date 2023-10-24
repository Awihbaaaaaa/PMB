
import matplotlib.pyplot as plt
import numpy as np

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
    ax.set_title(f'Radar view (r={r}, theta={theta} degrees)')
    ax.grid(True)
    
    return fig, ax