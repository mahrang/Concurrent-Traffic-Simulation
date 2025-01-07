# Concurrent Traffic Simulation

I built a traffic simulation where vehicles move concurrently along streets with traffic lights using multithreading.

Vehicles cross intersections when traffic lights are green and stop at intersections where traffic lights are red.

The traffic lights are the red and green dots, indicating red and green lights. 

The other circles are the vehicles.

In Paris:


https://github.com/user-attachments/assets/6c40b21e-ecb4-4752-a016-f1bc7c19b39e


In NYC:

![NYC](https://github.com/user-attachments/assets/093db493-518a-4db4-828d-5494d9f70ead)


## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./traffic_simulation`.
