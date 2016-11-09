cmake_minimum_required(VERSION 3.5)

project(VRPN-OpenVR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(
    vendor/openvr/headers
    vendor/vrpn/quat
    vendor/vrpn
)

add_executable(shingles
    VRPN-OpenVR/main.cpp
    VRPN-OpenVR/vrpn_Server_OpenVR.cpp
    VRPN-OpenVR/vrpn_Tracker_OpenVR.cpp
    VRPN-OpenVR/vrpn_Tracker_OpenVR_HMD.cpp
    VRPN-OpenVR/vrpn_Tracker_OpenVR_Controller.cpp
    )