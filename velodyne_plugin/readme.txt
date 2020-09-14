编译：
在./build下运行
cmake ..
make

在编译完成运行之前每个终端都要运行
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/velodyne_plugin/build

运行
在./build目录下打开两个终端
gazebo --verbose ../velodyne.world
./vel 2

原教程http://gazebosim.org/tutorials?cat=guided_i&tut=guided_i5
