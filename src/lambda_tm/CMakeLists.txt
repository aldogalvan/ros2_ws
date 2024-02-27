cmake_minimum_required(VERSION 3.8)
project(lambda_tm)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# Ensure that the Force Dimension SDK can be found, if the environmental 
# variable is set.
if(DEFINED ENV{ForceDimensionSDK_DIR})
    set(ForceDimensionSDK_DIR $ENV{ForceDimensionSDK_DIR})
endif()


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)


add_executable(lambda_publisher src/lambda_publisher.cpp)
add_executable(lambda_force src/lambda_force.cpp)
include_directories(/home/agalvan-admin/sdk-3.15.0/include)

# *****************************************************************************
# Find the Force Dimension SDK dependency.
# *****************************************************************************

# Ensure that the Force Dimension SDK can be found, if the environmental
# variable is set.

# Set ForceDimensionSDK_DIR environment variable
set(ForceDimensionSDK_DIR /home/agalvan-admin/sdk-3.15.0/)

if(DEFINED ENV{ForceDimensionSDK_DIR})
    set(ForceDimensionSDK_DIR $ENV{ForceDimensionSDK_DIR})
endif()

# Find the SDK.
find_package(ForceDimensionSDK REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb REQUIRED libusb-1.0)

# If the import of the Force Dimension SDK failed, then issue a message.
if(NOT ForceDimensionSDK_FOUND)
    message(ERROR "Set Force Dimension SDK base path via the environmental variable ForceDimensionSDK_DIR, or via the argument:  cmake -D ForceDimensionSDK_DIR=/path/to/sdk.")
endif()

#target_link_libraries(lambda_publisher
#	${HOME}/agalvan-admin/sdk-3.15.0/lib/release/lin-x86_64-gcc/libdhd.a
#	${HOME}/agalvan-admin/sdk-3.15.0/lib/release/lin-x86_64-gcc/libdrd.a
#	${HOME}/agalvan-admin/sdk-3.15.0/lib/release/lin-x86_64-gcc/libdrd.so.3.15.0
#    ${HOME}/agalvan-admin/sdk-3.15.0/lib/release/lin-x86_64-gcc/libdhd.so.3.15.0)

#target_link_libraries(lambda_publisher PUBLIC ${FORCE_DIMENSION_SDK_LIBRARIES})

# Add dependencies to the executable
ament_target_dependencies(lambda_publisher
    rclcpp
    geometry_msgs
    ForceDimensionSDK
    libusb
    Eigen3)

ament_target_dependencies(lambda_force
        rclcpp
        geometry_msgs
        ForceDimensionSDK
        libusb)

install(TARGETS
    lambda_publisher
    DESTINATION lib/${PROJECT_NAME})

install(TARGETS
        lambda_force
        DESTINATION lib/${PROJECT_NAME})

# Install launch files.
install(DIRECTORY
        launch
        DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
