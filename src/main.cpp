#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "std_msgs/String.h"
#include "actionlib/client/simple_action_client.h"

#include <sstream>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

geometry_msgs::Twist receive_msg;
bool receive_msg_flag = false;

move_base_msgs::MoveBaseGoal receive_move_base_msg;
bool receive_move_base_msg_flag = false;

void cmd_vel_callback(const geometry_msgs::Twist& msg){
    receive_msg_flag = true;
    receive_msg = msg;
}

void move_base_goal_callback(const move_base_msgs::MoveBaseGoal& msg){
    receive_move_base_msg_flag= true;
    receive_move_base_msg = msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "freespace_multi_msg");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1000);
  ros::Subscriber sub = n.subscribe("freespace_detection/cmd_vel", 1000, cmd_vel_callback);
  ros::Subscriber move_base_goal_sub = n.subscribe("freespace_detection/move_base_goal", 1000, move_base_goal_callback);
  MoveBaseClient ac("move_base", true);

  while(!ac.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    if(receive_msg_flag) {
        pub.publish(receive_msg);
        receive_msg_flag = false;
    }

    if(receive_move_base_msg_flag) {
        receive_move_base_msg_flag = false;
        ac.sendGoal(receive_move_base_msg);
    }


    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}

