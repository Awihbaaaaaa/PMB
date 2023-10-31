#################################################################
#################################################################
# Note that this code should be executed in an UBUNTU environment
#################################################################
#################################################################
""" 
import rosbag
import csv

# sensor_msgs is a tool from ROS for manipulating data from sensors, the sensor messages
from sensor_msgs.msg import PointCloud2

# Point clouds is tool to create, manipulate, and process point cloud messages
import sensor_msgs.point_cloud2 as pc2

import matplotlib.pyplot as plt

from plot_circle_slice import plot_circle_slice

# Create the figure for polar coordinates
fig, ax = plot_circle_slice(100,50)

# Enable interactive mode for the figures
plt.ion()

# create scatter for reseting the graph at each time step
scatter = ax.scatter([],[],c='red', label="Radar data")

# Open the ros bag
bag = rosbag.Bag('movingCar.bag')

# Specify the topic name want to be extracted
topic_name = '/radar_0'
i = 0
"""
""" 
The following command will check if there is a csv file named ros_data.csv 
so it will open it, otherwise it will create a new one. Once the with part is done
the csv fill will get saved.
"""
"""
with open('ros_data.csv', 'w', newline='') as csvfile:
    # Create a writer object 
    csv_writer = csv.writer(csvfile)

    # Fille the first line with the fields names
    csv_writer.writerow(['x', 'y', 'Vel'])
    
    # Extract the radar data by reading messages with topic /radar_0
    for topic, msg, t in bag.read_messages(topics=[topic_name]):
        print(type(msg))

        i = i+1
        print(i)

        # Read points from a L{sensor_msgs.PointCloud2} message
        # It returns a generator which yields a list of values for each point.
        #Extract the fields x, y, velocity from the message
        pc_data = pc2.read_points(msg, field_names=("x", "y", "velocity"), skip_nans=True)
        # print(pc_data)
        # Process the point cloud data
        x=[]
        y=[]
        velocity=[]
        for point in pc_data:
            #print(point)
            temp_x, temp_y, temp_velocity = point  # Extract x, y, and z coordinates
            x.append(temp_x)
            y.append(temp_y)
            velocity.append(temp_velocity)
            #print(x,y,velocity)
            # Process the x, y, z data as needed
        
        csv_writer.writerow([x, y, velocity])

        scatter.remove()
        scatter = ax.scatter(x,y, c='red', label="Data Point")
        plt.draw()
        plt.pause(0.1)
        #print(type(msg))
        x = msg.fields
        
    #print(f"Topic: {topic}, Message: {msg}, Timestamp: {t}")
    
plt.ioff()
plt.show()
bag.close()

"""