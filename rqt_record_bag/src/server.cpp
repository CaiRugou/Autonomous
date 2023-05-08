#include <ros/ros.h>
#include <iostream>
#include <cstdlib>
#include <dynamic_reconfigure/server.h>
#include <record_bag/paramConfig.h>

std::atomic_short g_Record{0};

void callback(record_bag::paramConfig &config, uint32_t level) {
    std::cout << "topic: " << config.record_topic << std::endl;
    int result = -1;
    std::string node_name = " __name:=" + config.node_name;

    if (config.record_flag == 1 && g_Record == 0) {
        
        std::string path = config.record_path;
        std::string topics = config.record_topic;
        std::string cmd_str = "gnome-terminal -x bash -c 'rosbag record -o " +
            path + " " + topics + node_name + "'";
        result = system(cmd_str.c_str()); 
    }
    else if (config.record_flag == 0 && g_Record == 1) {
        ros::V_string v_nodes;
        ros::master::getNodes(v_nodes);
        
        std::string node_ = "/" + config.node_name;
        auto it = std::find(v_nodes.begin(), v_nodes.end(), node_.c_str());
        if (it != v_nodes.end()) {
            std::string cmd_str = "rosnode kill " + node_;
            result = system(cmd_str.c_str()); 
        }   
    }
    
    g_Record = config.record_flag;    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "record_bag");
    dynamic_reconfigure::Server<record_bag::paramConfig> server;
    dynamic_reconfigure::Server<record_bag::paramConfig>::CallbackType func;
    func = boost::bind(&callback, _1, _2);
    server.setCallback(func);

    ros::spin();
    return 0;
}