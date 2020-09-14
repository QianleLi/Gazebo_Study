# Gazebo_Study  

## Preface  
Recently I started doing research with professor Yi Guo at Stevens. My first task was to get the maximum velocity of all car robot models in Gazebo, which was the reason why I learnt those stuff about Gazebo topics. This is a repository to keep notes about Gazebo study.
I used Ubuntu 18.04 ,ROS Melodic and Gazebo 9.
## Index

- [gz](README.md#gz)  
- [Plugins](README.md#Plugins)
- [world file](README.md#world-file)
- [Existing plugins](README.md#Existing-plugins)
- [Build and test](README.md#Build-and-test)
- [Publisher and subscriber](README.md#Publisher-and-subscriber)
- [Useful Resources](README.md#Useful-Resources)
- [Gazebo ros pakage](README.md#Gazebo-ros-package)

## gz

`gz` is a command that can help users to make changes to world, model, joint, physics and other properties in Gazebo. You can see the manual [here](http://manpages.ubuntu.com/manpages/bionic/man1/gz.1.html).  
For example, if you start gazebo from a terminal and insert a Pioneer2DX model, then in a new terminal run `gz joint -m pioneer2dx -j left_wheel_hinge --vel-t 1.0 --vel-p 1.0`. You will see the robot rotating in the environment.

## Plugins

I followed the [official tutorial](http://gazebosim.org/tutorials?cat=guided_i&tut=guided_i1) and the velodyne_plugin package is the plugin I built. Here I just list some notices for myself.  
### world file

Typical world file is shown as following.  
You may use different models and add more plugins.  
**Be careful about those names.**
```
<?xml version="1.0" ?>
<sdf version="1.5">
  <world name="default">
    <!-- A global light source -->
    <include>
      <uri>model://sun</uri>
    </include>
    <!-- A ground plane -->
    <include>
      <uri>model://ground_plane</uri>
    </include>

    <!-- A testing model  -->
    <model name="pioneer2dx">
      <include>
        <uri>model://pioneer2dx</uri>
      </include>
    </model>

  </world>
</sdf>
```
### Existing plugins

If you are using existing plugins, you have no need to build your own plugins. Therefore, the CMakeLists.txt should looks like this after you created your plugin source file.  
```
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Find Gazebo
find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")
```

### Build and test

Standard procedure:
```
cd ~/[package_name]
mkdir build
cd build
cmake ..
make
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/[package_name]/build
gazebo --verbose ../velodyne.world
```
## Publisher and subscriber

Two packages (velodyne_plugin and gazebo_drive_test) are examples. As I mentioned before, velodyne_plugin is a package built by following tutorials. The gazebo_drive_test package has a publisher to publish velocity message to Pioneer2DX robot which contains a DiffDrivePlugin.  
## Useful Resources

I found [a github repository](https://github.com/osrf/gazebo) which contains many existing plugin source files and other useful resources.
## Gazebo ros pakage  

When I was trying to drive Pioneer3AT robot as a Gazebo standalone model, I got error messages like tihs.   
```
[Err] [SkidSteerDrivePlugin.cc:51] Unable to find the right_front joint in model pioneer3at.
[Err] [SkidSteerDrivePlugin.cc:51] Unable to find the right_rear joint in model pioneer3at.
[Err] [SkidSteerDrivePlugin.cc:51] Unable to find the left_front joint in model pioneer3at.
[Err] [SkidSteerDrivePlugin.cc:51] Unable to find the left_rear joint in model pioneer3at.
```
After trying to find a way to fix this, I could not find a way out, but during that process I found out that I can use plugins of `gazebo_ros_pkgs` and tried and finally I can drive those SDF models.  
The following two sections are examples of use `gazebo_ros_diff_drive` and `gazebo_ros_skid_steer_drive`.    

- **Note that numbers in the following examples are not accurate, I am only take a note at the method here.**  
- **Pay attention to name of joints.**  
### Use gazebo ros diff drive in Pioneer2DX

Put the following code into the model.sdf file of pioneer2dx, right before the `</model>` attribute.
```
<plugin name="differential_drive_controller" filename="libgazebo_ros_diff_drive.so">
	<rosDebugLevel>Debug</rosDebugLevel>
	<publishWheelTF>true</publishWheelTF>
	<robotNamespace>/</robotNamespace>
	<publishTf>1</publishTf>
	<publishWheelJointState>true</publishWheelJointState>
	<alwaysOn>true</alwaysOn>
	<updateRate>100.0</updateRate>
	<legacyMode>true</legacyMode>
	<leftJoint>left_wheel_hinge</leftJoint>
	<rightJoint>right_wheel_hinge</rightJoint>
	<wheelSeparation>0.11</wheelSeparation>
	<wheelDiameter>0.22</wheelDiameter>
	<broadcastTF>1</broadcastTF>
	<wheelTorque>30</wheelTorque>
	<wheelAcceleration>1.8</wheelAcceleration>
	<commandTopic>cmd_vel</commandTopic>
	<odometryFrame>odom</odometryFrame> 
	<odometryTopic>odom</odometryTopic> 
	<robotBaseFrame>base_footprint</robotBaseFrame>
</plugin>
```
Open a terminal and run `rosrun gazebo_ros gazebo` to open Gazebo. Now insert pioneer2dx model manually. Open a new terminal and run `rostopic list` and you will see two topics `cmd_vel` and `odom`. 
I build a ros package consists of a publisher and a subscriber. The publisher publish velocity to topic `cmd_vel` and the subscriber get the velocity of the robot from topic`odom`. You can see the robot start moving after the publisher starts working.
### Use gazebo ros skid steer drive in Pioneer3AT
```
<plugin name="skid_steer_drive_controller" filename="libgazebo_ros_skid_steer_drive.so">
	<alwaysOn>true</alwaysOn>
	<updateRate>100.0</updateRate>
	<robotNamespace>/</robotNamespace>
	<leftFrontJoint>left_front</leftFrontJoint>
	<rightFrontJoint>right_front</rightFrontJoint>
	<leftRearJoint>left_rear</leftRearJoint>
	<rightRearJoint>right_rear</rightRearJoint>
	<wheelSeparation>0.2</wheelSeparation>
	<wheelDiameter>0.1</wheelDiameter>
	<torque>2</torque>
	<commandTopic>cmd_vel</commandTopic>
	<robotBaseFrame>base_link</robotBaseFrame>
	<broadcastTF>1</broadcastTF>
</plugin>
```
Do the same things I mentioned in the last section.
