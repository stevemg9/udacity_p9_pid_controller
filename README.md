# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

## Result Video
[PID Controller](https://youtu.be/HHxhYMPGCbY)

---
## Project Discussion
In this project we created a PID controller to drive a car on a simulated track.  The goal of the project was to create, the controller in C++, and find a way to tune the PID hyperparameters to successfully drive a lap around the track without veering off the road.

The PID controller consists of 3 components each with their own coefficient (or hyperparameter).  The first, and seemingly most important, component is the proportionality (P) component.  The P component adjusts the signal in the direction opposite of the error and by a value that is proportional to the error.  So, if your car is veering off to the right, the P term in the PID controller will begin to correct back to the left.  The Integral (I) component was the least important component for this project.  The I component will adjust your signal in proportion to the total error over time.  Therefore, the I component will correct long term error such as drift.  In this project we had no steering or throttle drift, so it was not really needed.  Also, since the track was circular I found that the I component tended to grow in one direction which actually seemed to cause a little bit of drift - this just led to less than ideal behavior as the simulation went on so I left the I term zero for both the steering and throttle controllers.  Finally the Differential (D) component, adjusts your signal in a way that is proportional to the difference in error between the current and previous time step.  So as your P component is correcting error, the error should be decreasing.  In this case the differential term will the have opposite sign of the P term (since: smaller error - larger error = negative value).  This will cause the overall error compensation to decrease as the error gets smaller.  The effect of this on our driving simulator is that it prevents the vehicle from overshooting and oscillating back and fourth across the lane center - as the vehicle gets closer to the lane center, the D term will decrease the correction value from the P term, thus giving us a much smoother driving experience.

Since the PID controller was a pretty simple implementation, the main part of this project was tuning the PID coefficients for a successful lap.  I implemented a PID controller for both steering and throttle and "trained" the separately (but there was a good amount of back and fourth).  I found that the I component needed to be very small as it would cause drift over time otherwise.  For the throttle it had to be zero because the throttle PID took the absolute value of the CTE - I did this because for throttle we don't care is our error is to the left or right, if we are off target we want to slow down regardless.  Because of this, the sum of the error over time would constantly grow over time leading to a constant decrease in throttle.  In order to train the and P and D coefficients, I implemented Twiddle in the PID class.  I found Twiddle to help, but in order to even run Twiddle I needed to find values that would at least keep the car on the track.  Because of this, I found myself hand tuning the parameters based on the intuition I mentioned above.  Once I was close, I used the Twiddle algorithm to fine tune the parameters.

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

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`.

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
