#include "vrpn_Tracker_OpenVR.h"
#include <openvr.h>
#include <quat.h>
#include <iostream>

vrpn_Tracker_OpenVR::vrpn_Tracker_OpenVR(const std::string& name, vrpn_Connection* connection, vr::IVRSystem * vr) :
	vrpn_Tracker(name.c_str(), connection), vr(vr)
{
	// Initialize the vrpn_Tracker
	vrpn_Tracker::num_sensors = 1;
}

void vrpn_Tracker_OpenVR::updateTracking(vr::TrackedDevicePose_t *pose) {
	if (!pose->bPoseIsValid) {
		return;
	}
	if (pose->eTrackingResult != vr::TrackingResult_Running_OK) {
		switch (pose->eTrackingResult) {
		case vr::TrackingResult_Uninitialized:
			std::cerr << "Uninitialized" << std::endl;
		break;
		case vr::TrackingResult_Calibrating_InProgress:
			std::cerr << "Calibrating (In Progress)" << std::endl;
		break;
		case vr::TrackingResult_Calibrating_OutOfRange:
			std::cerr << "Calibrating (Out of Range)" << std::endl;
		break;
		case vr::TrackingResult_Running_OK:
			std::cerr << "Running (OK)" << std::endl;
		break;
		case vr::TrackingResult_Running_OutOfRange:
			std::cerr << "Running (Out of Range)" << std::endl;
		break;
		default:
			std::cerr << "Unknown trackign result" << std::endl;
		break;
		}
	}

	// Sensor, doesn't change since we are tracking individual devices
	d_sensor = 0;

	// Position
	pos[0] = pose->mDeviceToAbsoluteTracking.m[0][3];
	pos[1] = pose->mDeviceToAbsoluteTracking.m[1][3];
	pos[2] = pose->mDeviceToAbsoluteTracking.m[2][3];

	// Quaternion
	q_matrix_type matrix;
	ConvertSteamVRMatrixToQMatrix(pose->mDeviceToAbsoluteTracking, matrix);
	q_from_col_matrix(d_quat, matrix);

	vrpn_gettimeofday(&timestamp, NULL);
	char msgbuf[1000];
	vrpn_int32 len = vrpn_Tracker::encode_to(msgbuf);
	if (d_connection->pack_message(len, timestamp, position_m_id, d_sender_id, msgbuf, vrpn_CONNECTION_LOW_LATENCY)) {
		std::cerr << "Can't write message" << std::endl;
	}
}

void vrpn_Tracker_OpenVR::mainloop() {
	server_mainloop();
}

void vrpn_Tracker_OpenVR::server_mainloop() {
	vrpn_Tracker::server_mainloop();
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