// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <thread>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    auto sub_opt = rclcpp::SubscriptionOptions();
    callback_group_sub_ = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::Reentrant);
    sub_opt.callback_group = callback_group_sub_;

    subscription1_ = this->create_subscription<std_msgs::msg::String>(
      "topic1",
      10,
      [this](std_msgs::msg::String::UniquePtr msg) {
        RCLCPP_INFO(this->get_logger(), "topic1 '%s'", msg->data.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(5));
        RCLCPP_INFO(this->get_logger(), "topic1 '%s' -- leave", msg->data.c_str());
      },
      sub_opt);
    subscription2_ = this->create_subscription<std_msgs::msg::String>(
      "topic2",
      10,
      [this](std_msgs::msg::String::UniquePtr msg) {
        RCLCPP_INFO(this->get_logger(), "topic2 '%s'", msg->data.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(5));
        RCLCPP_INFO(this->get_logger(), "topic2 '%s' -- leave", msg->data.c_str());
      },
      sub_opt);
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription1_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription2_;
  rclcpp::callback_group::CallbackGroup::SharedPtr callback_group_sub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::executor::ExecutorArgs(),5,true);
  auto node = std::make_shared<MinimalSubscriber>();
  executor.add_node(node);
  //rclcpp::spin(std::make_shared<MinimalSubscriber>());
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
