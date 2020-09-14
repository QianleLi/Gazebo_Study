#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

namespace gazebo
{
  /// \brief A plugin to control a Velodyne sensor. 建立一个插件
  class VelodynePlugin : public ModelPlugin   //继承了ModelPlugin
//https://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1ModelPlugin.html#ad0e19425716e57661491bae91d5c4b72
  {
    /// \brief Constructor
    public: VelodynePlugin() {}

    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    //当插件被插入仿真时，这个load函数被gazebo调用
	//第一个参数是指向被插入模型的指针
	//第二个参数是指向插件SDF元素的指针
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // Safety check
      if (_model->GetJointCount() == 0)
      {
        std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
        return;
      }

      // Store the model pointer for convenience.
	  //this是一个指向类VelodynePlugin的指针。
	  //下面的model, joint, pid, node 和sub都是在class最后创建的
      this->model = _model;

      // Get the first joint. We are making an assumption about the model
      // having one joint that is the rotational joint.
	  //在这个例子中默认只有一个joint,所以只取GetJoints()[0]
      this->joint = _model->GetJoints()[0];

      // Setup a P-controller, with a gain of 0.1.
	  //设置PID控制器
      this->pid = common::PID(0.1, 0, 0);

      // Apply the P-controller to the joint.
	  //把PID控制器应用到joint
      this->model->GetJointController()->SetVelocityPID(
          this->joint->GetScopedName(), this->pid);

      // Default to zero velocity
      double velocity = 0;

      // Check that the velocity element exists, then read the value
	  //检查是否有速度元素，接着读取速度值
      if (_sdf->HasElement("velocity"))
        velocity = _sdf->Get<double>("velocity");

      this->SetVelocity(velocity);

      // Create the node
	  //建立节点
      this->node = transport::NodePtr(new transport::Node());
      this->node->Init(this->model->GetWorld()->Name());

      // Create a topic name
      std::string topicName = "~/" + this->model->GetName() + "/vel_cmd";

      // Subscribe to the topic, and register a callback
	  //收听节点，调用回调函数
      this->sub = this->node->Subscribe(topicName,
         &VelodynePlugin::OnMsg, this);
    }

    /// \brief Set the velocity of the Velodyne
    /// \param[in] _vel New target velocity
	//用来设置速度的函数
    public: void SetVelocity(const double &_vel)
    {
      // Set the joint's target velocity.
      this->model->GetJointController()->SetVelocityTarget(
          this->joint->GetScopedName(), _vel);
    }

    /// \brief Handle incoming message
    /// \param[in] _msg Repurpose a vector3 message. This function will
    /// only use the x component.
	//处理受到的信息，这是个回调函数
    private: void OnMsg(ConstVector3dPtr &_msg)
    {
      this->SetVelocity(_msg->x());
    }

    /// \brief A node used for transport
    private: transport::NodePtr node;

    /// \brief A subscriber to a named topic.
    private: transport::SubscriberPtr sub;

    /// \brief Pointer to the model.
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr joint;

    /// \brief A PID controller for the joint.
    private: common::PID pid;
  };

  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(VelodynePlugin)
}
#endif
