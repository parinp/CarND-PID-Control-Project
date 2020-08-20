# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

This project focuses on developing the PID controller for the car to be able to navigate the track without leaving the track at all.  A Manual tuning we was done with a bit of twiddle to get a sense on how to tune the hyperparameters.

## Parameter Behavior

I was able to observe the phenomena of each component while tuning.

P component:  
P componenet is the direct adjustment to the error, if the car is too right it will turn left and if it is too left it will turn right.  Using only this controller I have found that the car will be able to make quick adjustments to the error.  However, using too high of a value will make the car overshoot drastically and it will swing from side to side.  

I componenet:  
I component is used in order to adjust for the difference in error from the center line.  What this means is that, we may have a very stable car that can drive the track but it may not drive at the center of the track.  This will help adjust and get the car as close to the center line as possible.

Including only this component will make the car turn off the track the moment the simulator is started.

D component:  

This component reduces or increases the effect of the adjustment to the error based on how far the error is from the center line.  What this means is that the controller will make the car smoothen out when approaching center line and will reduce the overshoot caused by the P component.

## Parameter Tuning

The final parameters were chosen from a combination of both **Manual** and **Twiddle** tuning.  The PID was first tuned manually, I chose to do this first as I am more familiar with manual tuning and also after which it will make Twiddle tuning converge a lot easier.  I ended up with values of `Kp = 0.12`, `Ki = 0.0.0005`, `Kd = 3.50`. 

Then I activated the twiddle algorithm to help converge around those values.  The final parameters are:

Coefficients | Kp | Ki | Kd |
:-----------:|:--:|:--:|:--:|
Value|1.10|0.005|3.50|

## Reflection

The controller behaved as expected and I will that there could be a lot of improvements with the twiddle algorithm I implemented.  I was not able to decide on the error to use and hence, I used the travel distance as a measure of error and to use *cte_error* if and only if the simulator reseted near the same point.

This project has been fun to play around and will definitely love to find improvements to the controller as well as methods to tuning the hyperparameters.

Udacity README for quick setup
---


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

