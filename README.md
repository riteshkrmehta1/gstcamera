# Introduction

This program is a simple gstreamer wrapper that allows a couple of ioctls to be called on a V4L2 camera interface. This will work with any USB cameras.
Also this will work in any Linux based destributions.

# Prerequisite

Please make sure gstreamer based packages installed in the system.
* sudo apt-get install libgstreamer1.0-0 gstreamer1.0-dev  

# Compilation

* make

# Stream the attached camera
* Run the gstcamera for 10 seconds.
* ./gstcamera wt10
