#include "vrpn_Tracker_OpenVR_Controller.h"

vrpn_Tracker_OpenVR_Controller::vrpn_Tracker_OpenVR_Controller(const std::string& name, vrpn_Connection* connection, vr::IVRSystem * vr) :
	vrpn_Tracker_OpenVR(name.c_str(), connection, vr),
	vrpn_Analog(name.c_str(), connection),
	vrpn_Button_Filter(name.c_str(), connection)
{
	// Initialize the vrpn_Analog
	vrpn_Analog::num_channel = vr::k_unControllerStateAxisCount * 2; // * 2 for x&y
	for (auto i = 0; i < vrpn_Analog::num_channel; i++) {
		vrpn_Analog::channel[i] = vrpn_Analog::last[i] = 0;
	}

	// Initialize the vrpn_Button_Filter
	vrpn_Button_Filter::num_buttons = vr::k_EButton_Max;
	for (auto i = 0; i < vrpn_Button_Filter::num_buttons; i++) {
		vrpn_Button_Filter::buttons[i] = vrpn_Button_Filter::lastbuttons[i] = 0;
	}
}

void vrpn_Tracker_OpenVR_Controller::mainloop() {
    vrpn_gettimeofday( &(vrpn_Tracker_OpenVR::timestamp), NULL );
	vrpn_Tracker_OpenVR::mainloop();

	vrpn_gettimeofday( &(vrpn_Analog::timestamp), NULL );
	vrpn_Analog::report_changes();


    vrpn_gettimeofday( &(vrpn_Button_Filter::timestamp), NULL );
	vrpn_Button_Filter::report_changes();
}

void vrpn_Tracker_OpenVR_Controller::updateController(vr::TrackedDeviceIndex_t unTrackedDevice) {
	// Analog & Buttons
	if (vr->GetTrackedDeviceClass(unTrackedDevice) == vr::TrackedDeviceClass_Controller) {
		vr->GetControllerState(unTrackedDevice, &pControllerState);

		for (unsigned int buttonId = 0; buttonId < vr::k_EButton_Max; ++buttonId) {
			uint64_t mask = vr::ButtonMaskFromId(static_cast<vr::EVRButtonId>(buttonId));
			vrpn_Button_Filter::buttons[buttonId] = static_cast<unsigned char>(((mask & pControllerState.ulButtonTouched) == mask) ? 2 : 0);
			vrpn_Button_Filter::buttons[buttonId] = static_cast<unsigned char>(((mask & pControllerState.ulButtonPressed) == mask) ? 1 : vrpn_Button_Filter::buttons[buttonId]);
		}

		for (unsigned int axisId = 0; axisId < vr::k_unControllerStateAxisCount; ++axisId) {
			vrpn_Analog::channel[axisId * 2] = pControllerState.rAxis[axisId].x;
			vrpn_Analog::channel[axisId * 2 + 1] = pControllerState.rAxis[axisId].y;
		}
	}
}
