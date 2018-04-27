#ifndef AKIT_PICK_PLACE_H
#define AKIT_PICK_PLACE_H
#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <tf/transform_datatypes.h>
#include <tf/tf.h>
#include <visualization_msgs/Marker.h>

#define UP true
#define DOWN false

class akit_pick_place {

private:

  //ros
  ros::NodeHandle nh;
  ros::Publisher marker_pub;
  visualization_msgs::Marker marker;

  //akit stuff
  std::string BASE_LINK;
  std::string PLANNING_GROUP_NAME;
  std::string EEF_PARENT_LINK; //last link in planning group kinematic chain "quickcoupler"
  std::string EEF_GROUP;
  double GRIPPER_JAW_LENGTH; //for cartesian motion
  double GRIPPER_LENGTH;   //for grasp generation
  double GRIPPER_SIDE_LENGTH;

  //cartesian motion
  bool gripperSuccess;
  bool akitSuccess;
  bool setFromGraspGenerator;

  //akit pick & cartesian stuff
  geometry_msgs::Pose pre_grasp_pose;
  geometry_msgs::Pose pre_place_pose;
  geometry_msgs::Pose cartesian_pose;
  geometry_msgs::Pose grasp_pose;
  moveit_msgs::RobotTrajectory trajectory;
  std::vector<geometry_msgs::Pose> waypoints;
  std::vector<geometry_msgs::Pose> grasp_pose_vector;

  //MoveIt! stuff
  moveit::planning_interface::MoveGroupInterface *akitGroup;
  moveit::planning_interface::MoveGroupInterface *gripperGroup;
  moveit::planning_interface::PlanningSceneInterface planningSceneInterface;
  moveit::planning_interface::MoveGroupInterface::Plan MotionPlan;
  moveit::planning_interface::MoveGroupInterface::Plan gripperMotionPlan;
  moveit::core::RobotStatePtr gripperState;
  std::vector<double> gripperJointPositions;
  const robot_state::JointModelGroup *akitJointModelGroup;
  const robot_state::JointModelGroup *gripperJointModelGroup;

public:

  //constructors
  akit_pick_place(std::string planning_group_, std::string eef_group_,
                  std::string base_link_, std::string eef_parent_link_, double gripper_length_,
                  double gripper_jaw_length_,double gripper_side_length_, bool set_from_grasp_generator_);
  akit_pick_place();
  ~akit_pick_place();

  //methods
  void initialize();
  void setDefaultPlanningGroup();
  void setPlanningGroup(std::string planning_group_);
  void setGripperGroup(std::string eef_group_);
  void setBaseLink(std::string base_link_);
  void setPreGraspPose(geometry_msgs::Pose preGraspPose);
  void setPrePlacePose(geometry_msgs::Pose prePlacePose);

  std::string getPlanningGroup();
  std::string getGripperGroup();
  std::string getBaseLink();

  bool generateGrasps(geometry_msgs::Pose block_pose_, double block_size_, bool visualize = true);
  bool generateGrasps(geometry_msgs::Pose cylinder_pose_, double cylinder_height_, double cylinder_radius_, bool visualize = true);
  bool generateGrasps(geometry_msgs::Pose cuboid_pose_, double cuboid_x_, double cuboid_y_, double cuboid_z_, bool visualize = true);
  bool visualizeGrasps();

  //choose best grasp -->later
  //rotate gripper body for closed gripper collisions --> later --> improve
  // transform from base frame to world frame -->later

  bool rotateGripper(bool plan_only = false);
  bool openGripper(bool plan_only = false);
  bool closeGripper(bool plan_only = false);
  bool executeCartesianMotion(bool direction);
  bool pick(std::string object_id);
  bool place(std::string object_id);
  bool pick_place(std::string object_id);

};

#endif // AKIT_PICK_PLACE_H
