# VRPN-OpenVR
OpenVR support for VRPN. Builds as a server that exposes OpenVR HMDs and controllers as as openvr/[hmd|controller]/[trackedDeviceId]. 
Each of which having the relevant vrpn devices that it supports: vrpn_Tracker for HMDs and vrpn_Tracker, crpn_Analog and vrpn_Button
for the controllers.

# Building
This server currently only builds in Visual Studio (2015 was used) as OpenVR is only supported in windows and we ran
into issues trying to build it with CMake under Cygwin. A CMake file generating a VS project was considered but was 
found to introduce too many steps for the end result still requiring switching to VS for the build.

Note: the current `CMakeLists.txt` is only a dummy to allow CLion to understand the project.

* Initialize the repository with submodules in order to retrieve OpenVR and VRPN

        git submodule update --init --recursive
        
* Open the `VRNP-OpenVR.sln` solution and build from the IDE

# Acknowledgements
This is still very much a work in progress and is by no means a complete and stable solution for using OpenVR with VRPN.
The primary goal of this server is to provide controller data to a Blender instance running on a separate Linux machine.