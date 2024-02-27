/* TODO
 * Install the necessary parts of the Force Dimension SDK where appropriate
 * Make sure the package.xml & CMakeLists are both correctly linking/compiling the Force Dimension sdk
 */

// C++ library headers
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <iomanip>

// ROS library headers
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

// Eigen3 (matrix to quat)
#include <Eigen/Geometry>

// Force Dimension (i.e. Lambda.7) headers
#include "dhdc.h"

using namespace std::chrono_literals;

class LambdaPublisher : public rclcpp::Node
{
public:
    LambdaPublisher() : rclcpp::Node("lambda_publisher")
    {
        // Creates publishers for the Lambda.7's position and velocity
        pose_publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("lambda_pose", 10);
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("lambda_velocity", 10);
        // Publishes messages periodically
        timer_ = this->create_wall_timer(100ms, [this]() {
            publishPoseMessage();
            publishVelocityMessage();
        });
    }
private:
    // Function to publish Lambda.7 pose
    void publishPoseMessage()
    {
        double px, py, pz;
        double matrix[3][3];
        dhdGetPositionAndOrientationFrame(&px, &py, &pz, matrix);
        Eigen::Map<Eigen::Matrix3d> orientation(reinterpret_cast<double*>(matrix));
        Eigen::Quaterniond quaternion(orientation);
        auto msg = geometry_msgs::msg::Pose();
        msg.position.x = px;
        msg.position.y = py;
        msg.position.z = pz;
        msg.orientation.x = quaternion.x();
        msg.orientation.y = quaternion.y();
        msg.orientation.z = quaternion.z();
        msg.orientation.w = quaternion.w();
        RCLCPP_INFO(this->get_logger(), "Publishing Position: '(%.2f, %.2f, %.2f)' & Orientation Quaternion(x, y, z, w): (%.2f, %.2f, %.2f, %.2f)", msg.position.x, msg.position.y, msg.position.z, msg.orientation.x, msg.orientation.y, msg.orientation.z, msg.orientation.w);
        pose_publisher_->publish(msg);
    }

    // Function to publish Lambda.7 velocity
    void publishVelocityMessage()
    {
        double vx, vy, vz;
        dhdGetLinearVelocity(&vx, &vy, &vz);
        double wx, wy, wz;
        dhdGetAngularVelocityRad(&wx, &wy, &wz);
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = vx;
        msg.linear.y = vy;
        msg.linear.z = vz;
        msg.angular.x = wx;
        msg.angular.y = wy;
        msg.angular.z = wz;
        RCLCPP_INFO(this->get_logger(), "Publishing Linear Velocity: '<%.2f, %.2f, %.2f>' & Angular Velocity '<%.2f, %.2f, %.2f>'", msg.linear.x, msg.linear.y, msg.linear.z, msg.angular.x, msg.angular.y, msg.angular.z);
        double fx, fy, fz;
        if(dhdGetForce(&fx, &fy, &fz) == 0) {
            RCLCPP_INFO(this->get_logger(), "Publishing Force: <%.2f, %.2f, %.2f>", fx, fy, fz);
        }
        velocity_publisher_->publish(msg);

    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr pose_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
};

int main(int argc, char** argv)
{
    // Open the first available device
    if(dhdOpen() < 0) {
        std::cout << "error: " << dhdErrorGetLastStr() << std::endl;
        return -1;
    }

    //ROS loop
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LambdaPublisher>());
    rclcpp::shutdown();

    // Close the connection to the device
    if(dhdClose() < 0) {
        std::cout << "error: " << dhdErrorGetLastStr() << std::endl;
    }
    return 0;
}

