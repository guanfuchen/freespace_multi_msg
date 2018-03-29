#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/String.h"

#include <sstream>

geometry_msgs::Twist receive_msg;
bool receive_msg_flag = false;

void cmd_vel_callback(const geometry_msgs::Twist& msg){
    receive_msg_flag = true;
    receive_msg = msg;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "freespace_multi_msg");

  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1000);
  ros::Subscriber sub = n.subscribe("freespace_detection/cmd_vel", 1000, cmd_vel_callback);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    if(receive_msg_flag) {
        pub.publish(receive_msg);
        receive_msg_flag = false;
    }


    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}

