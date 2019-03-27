#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <mutex>
#include <boost/thread.hpp>
#include <forward_list>
#include <vector>

namespace newScan
{

    struct scansubsription{

        ros::Subscriber scan_sub;
        ros::Publisher scan_pub;

        std::string robot_name;
    };

    class newscan{
        public:
            newscan();

            void topicSubscribe();
        private:
            ros::NodeHandle node_;          

            std::string robot_namespace_;
            std::string robot_scan_topic_;
            std::string new_scan_topic_;
            std::vector<std::string> robots_;
            boost::shared_mutex subsriptions_mutex;
            std::forward_list<scansubsription> subscriptions_;
            int subscriptions_size_;

            bool isRobotScantopic(const ros::master::TopicInfo& topic);
            void UpdateScan(const sensor_msgs::LaserScan::ConstPtr& msg, scansubsription& subscription);
            
    };
}
