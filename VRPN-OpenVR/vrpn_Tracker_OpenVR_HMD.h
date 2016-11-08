#pragma once

#include <openvr.h>
#include "vrpn_Tracker_OpenVR.h"

class vrpn_Tracker_OpenVR_HMD :
	public vrpn_Tracker_OpenVR
{
public:
	vrpn_Tracker_OpenVR_HMD() = delete;
	vrpn_Tracker_OpenVR_HMD(const std::string& name, vrpn_Connection* connection, vr::IVRSystem * vr);
	void mainloop();
	void server_mainloop();
private:
};

