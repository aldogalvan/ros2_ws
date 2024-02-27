/* TODO:
 <*> Write subscriber node
 <*> Write force publishing node
 <*> Integrate into package.xml, CMakeLists.txt, Config.cmake, launch file
 * Build & test
 *
 * Figure out about the logging on screen?
 */
#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include <iomanip>
#include <chrono>

// ROS library headers
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
// TODO: Import the necessary geometry_msgs files depending on what type the Sony device topic publishes

// Force Dimension headers
#include "dhdc.h"

using namespace std::chrono_literals;
using std::placeholders::_1;

class LambdaForce : public rclcpp::Node
{
public:
    LambdaForce() : rclcpp::Node("lambda_force") {
        //Creates subscribers to the TODO {Sony Device name}'s topic
        lambda_subscriber_ = this->create_subscription<geometry_msgs::msg::Vector3>("sony_topic", 10, std::bind(&LambdaForce::force_activator, this, _1)); // TODO: change "lambda_force_subscriber" to name of topic & add geometry_msgs type between <>
        timer_ = this->create_wall_timer(100ms, std::bind(&LambdaForce::check_connection, this)); //TODO: update the time for updatesd for the timer
    }
private:
    // Function to set the forces according to Sony's force feedback topic
    void force_activator(const geometry_msgs::msg::Vector3 & force) const // TODO: add type of argument (type of geometry_msgs object)
    {
         RCLCPP_INFO(this->get_logger(), "Received Force Vector: <%.2f, %.2f, %.2f>", force.x, force.y, force.z); // TODO: add name of variable of argument
         if (dhdSetForce(force.x, force.y, force.z) < 0){ // TODO: set force based on subscription inputs
             RCLCPP_ERROR(this->get_logger(), "Failed to set a force: %s", dhdErrorGetLastStr());
         }
    }

    void check_connection()
    {
        auto topic_info = this->get_topic_names_and_types();
        bool connected = std::any_of(topic_info.begin(), topic_info.end(), [this](const auto& info) {return info.first == "sony_topic";}); // TODO: change "sony_topic" for actual name

        if(!connected) {
            handle_disconnection();
        } else {
            handle_connection();
        }
    }

    void handle_disconnection()
    {
        if(dhdEnableForce(DHD_ON) == 0) {
            RCLCPP_INFO(this->get_logger(), "Force Enabled.");
        }
// Turn off all forces
    double fx, fy, fz, tx, ty, tz, fg = 0;
    if(dhdSetForceAndTorqueAndGripperForce(fx, fy, fz, tx, ty, tz, fg) == 0) {
        RCLCPP_INFO(this->get_logger(), "dhdSetForceAndTorqueAndGripperForce(): %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f", fx, fy, fz, tx, ty, tz, fg);
    }


//
    double t0, t1, t2 = 0;
    if(dhdSetForceAndWristJointTorquesAndGripperForce(fx, fy, fz, t0, t1, t2, fg) == 0) {
        RCLCPP_INFO(this->get_logger(), "dhdSSetForceAndWristJointTorquesAndGripperForce(): %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f", fx, fy, fz, t0, t1, t2, fg);
    }
//
//        Turn off brakes
//    if (dhdSetBrakes(DHD_OFF) < 0) {// Sets Lambda.7 force feedback to default 0 force vector
//        RCLCPP_ERROR(this->get_logger(), "Failed to turn brakes off: %s", dhdErrorGetLastStr());
//    }
        RCLCPP_WARN(this->get_logger(), "Not connected to 'sony_topic'. Setting force feedback to <0, 0, 0> default."); // TODO: Replace sony_topic w/ actual name
    }

    void handle_connection()
    {
        RCLCPP_INFO(this->get_logger(), "Connected to 'sony_topic'. Receiving messages."); // TODO: Replace sony_topic w/ actual name
    }

    rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr lambda_subscriber_; // TODO: add type of argument (type of geometry_msgs object) between <>
    rclcpp::TimerBase::SharedPtr timer_;
//    rclcpp::Time last_message_time_;
//    const int timeout_threshold_ = 0.01;
};

int main(int argc, char ** argv) {
    // Open the first available device
    if(dhdOpen() < 0) {
        std::cout << "error: " << dhdErrorGetLastStr() << std::endl;
        return -1;
    }


    //ROS loop
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LambdaForce>());
    rclcpp::shutdown();

    // Close the connection to the device
    if(dhdClose() < 0) {
        std::cout << "error: " << dhdErrorGetLastStr() << std::endl;
    }
    return 0;
}