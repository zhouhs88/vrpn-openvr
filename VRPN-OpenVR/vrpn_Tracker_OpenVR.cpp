#include <iostream>
#include <string>
#include <quat.h>
#include "vrpn_Tracker_OpenVR.h"

vrpn_Tracker_OpenVR::vrpn_Tracker_OpenVR(const char* name, vrpn_Connection * cxn) :
	vrpn_Tracker(name, cxn),
	vrpn_Analog(name,cxn),
	vrpn_Button_Filter(name,cxn)
{
	// initialize the vrpn_Analog
	vrpn_Analog::num_channel = 2;
	for (auto i = 0; i < vrpn_Analog::num_channel; i++) {
		vrpn_Analog::channel[i] = vrpn_Analog::last[i] = 0;
	}

	// initialize the vrpn_Button_Filter
	vrpn_Button_Filter::num_buttons = 3;
	for (auto i = 0; i < vrpn_Button_Filter::num_buttons; i++) {
		vrpn_Button_Filter::buttons[i] = vrpn_Button_Filter::lastbuttons[i] = 0;
	}

	// VR
	vr::EVRInitError eError = vr::VRInitError_None;
	vr = std::unique_ptr<vr::IVRSystem>(vr::VR_Init(&eError, vr::VRApplication_Overlay));
	if (eError != vr::VRInitError_None)
	{
		vr = nullptr;
		std::cerr << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
	}
}


vrpn_Tracker_OpenVR::~vrpn_Tracker_OpenVR() {
	vr::VR_Shutdown();
}

void vrpn_Tracker_OpenVR::mainloop() {
	


	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	vr->GetDeviceToAbsoluteTrackingPose(
		vr::TrackingUniverseStanding,
		0/*float fPredictedSecondsToPhotonsFromNow*/,
		m_rTrackedDevicePose,
		vr::k_unMaxTrackedDeviceCount
	);

	for (vr::TrackedDeviceIndex_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++) {
		if (!m_rTrackedDevicePose[unTrackedDevice].bDeviceIsConnected) {
			continue;
		}
		if (vr->GetTrackedDeviceClass(unTrackedDevice) != vr::TrackedDeviceClass_Controller) {
			continue;
		}

		// Find out what time we received the new information, then send any
		// changes to the client.
		vrpn_gettimeofday(&_timestamp, NULL);
		vrpn_Tracker::timestamp = _timestamp;
		vrpn_Analog::timestamp = _timestamp;
		vrpn_Button::timestamp = _timestamp;

		// Position
		std::cout << std::to_string(m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[0][3]) << ","
			<< std::to_string(m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[1][3]) << ","
			<< std::to_string(m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[2][3]) << std::endl;

		d_sensor = unTrackedDevice;
		pos[0] = m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[0][3];
		pos[1] = m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[1][3];
		pos[2] = m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking.m[2][3];

		q_matrix_type matrix;
		ConvertSteamVRMatrixToQMatrix(m_rTrackedDevicePose[unTrackedDevice].mDeviceToAbsoluteTracking, matrix);
		q_from_col_matrix(d_quat, matrix);
		
		char msgbuf[1000];
		vrpn_int32 len = vrpn_Tracker::encode_to(msgbuf);
		if (d_connection->pack_message(len, _timestamp, position_m_id, d_sender_id, msgbuf, vrpn_CONNECTION_LOW_LATENCY)) {
			std::cerr << "Can't write message" << std::endl;
		}
		// Description of the next report to go out
		//vrpn_int32 d_sensor;              // Current sensor
		//vrpn_float64 pos[3], d_quat[4];   // Current pose, (x,y,z), (qx,qy,qz,qw)
		//vrpn_float64 vel[3], vel_quat[4]; // Cur velocity and dQuat/vel_quat_dt
		//vrpn_float64 vel_quat_dt;         // delta time (in secs) for vel_quat
		//vrpn_float64 acc[3], acc_quat[4]; // Cur accel and d2Quat/acc_quat_dt2
		//vrpn_float64 acc_quat_dt;         // delta time (in secs) for acc_quat
		//struct timeval timestamp;         // Current timestamp
		//vrpn_int32 frame_count;           // Current framecount
	}

	vrpn_Analog::report_changes();
	vrpn_Button::report_changes();

	server_mainloop();
	/*for (vr::TrackedDeviceIndex_t unTrackedDevice = 0; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
	std::cout << vr->GetTrackedDeviceClass(unTrackedDevice) << std::endl;
	if (vr->GetTrackedDeviceClass(unTrackedDevice) != vr::TrackedDeviceClass_Controller)
	continue;

	vr::VRControllerState_t state;
	vr::TrackedDevicePose_t pose;
	if (vr->GetControllerStateWithPose(vr::TrackingUniverseStanding, unTrackedDevice, &state, &pose))
	{
	if (!pose.bPoseIsValid) {
	continue;
	}

	if (pose.eTrackingResult != vr::TrackingResult_Running_OK) {
	continue;
	}
	}
	}*/



}

void vrpn_Tracker_OpenVR::ConvertSteamVRMatrixToQMatrix(const vr::HmdMatrix34_t &matPose, q_matrix_type &matrix) {
	matrix[0][0] = matPose.m[0][0];
	matrix[1][0] = matPose.m[1][0];
	matrix[2][0] = matPose.m[2][0];
	matrix[3][0] = 0.0;
	matrix[0][1] = matPose.m[0][1];
	matrix[1][1] = matPose.m[1][1];
	matrix[2][1] = matPose.m[2][1]; 
	matrix[3][1] = 0.0;
	matrix[0][2] = matPose.m[0][2];
	matrix[1][2] = matPose.m[1][2];
	matrix[2][2] = matPose.m[2][2];
	matrix[3][2] = 0.0;
	matrix[0][3] = matPose.m[0][3];
	matrix[1][3] = matPose.m[1][3];
	matrix[2][3] = matPose.m[2][3];
	matrix[3][3] = 1.0f;
}