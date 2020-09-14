# Gazebo_Study  

## Preface  
Recently I started doing research with professor Yi Guo at Stevens. My first task was to get the maximum velocity of all car robot models in Gazebo, which was the reason why I learnt those stuff about Gazebo topics. This is a repository to keep notes about Gazebo study.
I used Ubuntu 18.04 ,ROS Melodic and Gazebo 9.

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