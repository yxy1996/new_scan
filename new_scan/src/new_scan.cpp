#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <new_scan.h>
#include <thread>

namespace newScan{
newscan::newscan() : 
subscriptions_size_(0)
{
        ros::NodeHandle private_nh("~");
        private_nh.param<std::string>("robot_scan_topic",robot_scan_topic_,"scans");
        private_nh.param<std::string>("robot_namespace",robot_namespace_,"tb3");             //for example, we have tb3_0/scan, tb3_1/scan, tb3_2/scan, ...
        private_nh.param<std::string>("new_scan_topic",new_scan_topic_,"new_scan");
}
void newscan::topicSubscribe(){
        ros::master::V_TopicInfo topic_infos;       
        std::string robot_name;
        std::string scan_topic;

        ros::master::getTopics(topic_infos);     //push all topics in  

        for (const auto& topic : topic_infos){
            if(!isRobotScantopic(topic)){
                continue;
            }

        robot_name = ros::names::parentNamespace(topic.name);//will return tb3_0,  tb3_1,  ...
        
        if ( !(std::find(robots_.begin(),robots_.end(),robot_name) == robots_.end()) ){
             continue;                                                                         //if not found
        }
        //mutex
        {
            std::lock_guard<boost::shared_mutex> lock(subsriptions_mutex);
            subscriptions_.emplace_front();
            ++subscriptions_size_;
        }

        scansubsription& subscription = subscriptions_.front();
        robots_.push_back(robot_name);

        scan_topic = ros::names::append(robot_name, robot_scan_topic_);

        subscription.robot_name = robot_name;
        subscription.scan_sub = node_.subscribe<sensor_msgs::LaserScan>(scan_topic,
                     1000,[this,&subscription,&robot_name](const sensor_msgs::LaserScan::ConstPtr& msg)
                          {
                                UpdateScan(msg, subscription);
                          });
        }
}    
void newscan::UpdateScan(const sensor_msgs::LaserScan::ConstPtr& msg, scansubsription& subscription){
    std::string new_scan_name = ros::names::append(subscription.robot_name, "new_scan");
  //  std::cout<<robot_name<<std::endl;
    subscription.scan_pub = node_.advertise<sensor_msgs::LaserScan>(new_scan_name,10);
    sensor_msgs::LaserScan new_msg = *msg;
                             for(auto& range : new_msg.ranges)
                             {
                                 if(std::isinf(range) || range > new_msg.range_max)
                                 {
                                     range = new_msg.range_max - 0.01;
                                 }
                             }
                             subscription.scan_pub.publish(new_msg);
}

bool newscan::isRobotScantopic(const ros::master::TopicInfo& topic)  
{
        std::string topic_namespace = ros::names::parentNamespace(topic.name);
        bool is_scan_topic = 
            ros::names::append(topic_namespace, robot_scan_topic_) == topic.name;
        return is_scan_topic ;
}
}
int main(int argc, char** argv)
{
    ros::init(argc, argv, "laser_filter");
    newScan::newscan new_scan;

    new_scan.topicSubscribe();
    ros::spin();
    return 0;
}
