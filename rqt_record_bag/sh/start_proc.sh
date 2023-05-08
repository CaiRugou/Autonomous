#!/bin/bash

source ../install/setup.bash

rosrun record_data record_data_node

rosrun rqt_reconfigure rqt_reconfigure