#include <ignition/math/Pose3.hh>
#include <gazebo/gazebo_client.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_config.h>
#include <iostream>
#include <unistd.h>
#define DEG2RAD(x)  x*0.017453293   //从角度转为弧度
#define RAD2DEG(x)  x*57.29577951

/////////////////////////////////////////////////
int main(int _argc, char **_argv)
{
  // Load gazebo
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Publish to a Gazebo topic
  gazebo::transport::PublisherPtr pub =
    node->Advertise<gazebo::msgs::Pose>("~/pioneer2dx/vel_cmd");

  // Wait for a subscriber to connect
  pub->WaitForConnection();

  // Publisher loop...replace with your own code.
  double x_vel = 0;
  while (true)
  {
    // Throttle Publication
    gazebo::common::Time::MSleep(1000);

    // Generate a pose
    ignition::math::Pose3d pose(x_vel, 0.0, 0.0, 0.0, 0.0, DEG2RAD(40));

    // Convert to a pose message
    gazebo::msgs::Pose msg;
    gazebo::msgs::Set(&msg, pose);
    pub->Publish(msg);
	x_vel += 0.5;
  }

  // Make sure to shut everything down.
  gazebo::client::shutdown();
}
