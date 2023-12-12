# PMB Filter Implementation

The Poisson Multi-Bernoulli (PMB) filter is a probabilistic filtering algorithm used in multi-object tracking scenarios, particularly in the field of target tracking. It's designed to handle situations where the number of objects (targets) and their associations with measurements. The PMB filter combines concepts from the Poisson Point Process (PPP) distribution for representing the potentially untracked objects and the Multi Bernouli distribution for representing the tracked objects.

## Poisson Point Process (PPP) distribution
The "P" in PMB stands for the Poisson Point Process distribution. The Poisson distribution is commonly used for modeling the number of independent events in a certain time interval e.g., The number of pedestrian crossing a road during one hour. In MOT applications, the Poisson Point Process (PPP) is used to model objects with a discrete
number of instances, such as measurement points or the arrival of new potential objects. In the PMB filter, PPP is used extensively to model potential objects and how likely they are to exist. By modeling the undetected objects as PPP, effects such as increased likelihood of detecting new objects close to the edges of a sensor FOV is easily modeled.

## Multi-Bernoulli (MB) Distribution
The "MB" in PMB stands for the Multi-Bernoulli distribution. The Multi-Bernoulli distribution is a generalization of the Bernoulli distribution to multiple dimensions, making it suitable for modeling the state of multiple objects in a tracking scenario. In the PMB filter, the MB distribution is employed to represent the state of multiple targets, considering both their existence and state information.

