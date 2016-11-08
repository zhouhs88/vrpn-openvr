#include "vrpn_Tracker_OpenVR_HMD.h"

vrpn_Tracker_OpenVR_HMD::vrpn_Tracker_OpenVR_HMD(const std::string& name, vrpn_Connection* connection, vr::IVRSystem * vr) :
	vrpn_Tracker_OpenVR(name.c_str(), connection, vr)
{
}

void vrpn_Tracker_OpenVR_HMD::mainloop() {
	server_mainloop();
}

void vrpn_Tracker_OpenVR_HMD::server_mainloop() {
	vrpn_Tracker_OpenVR::server_mainloop();
}