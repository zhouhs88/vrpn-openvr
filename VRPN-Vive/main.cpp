#include <windows.h>
#include <iostream>
#include <openvr.h>

int main() {
    vr::EVRInitError eError = vr::VRInitError_None;
    vr::IVRSystem *vr = vr::VR_Init(&eError, vr::VRApplication_Scene);
    if ( eError != vr::VRInitError_None )
    {
        vr = nullptr;
        std::cout << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription( eError ) << std::endl;
        return false;
    }

	bool quit = false;
	while (!quit) {
		std::cout << "---" << std::endl;

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
			std::cout << m_rTrackedDevicePose[unTrackedDevice].vVelocity.v[0] << std::endl;
			Sleep(1000);
		}

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

    return 0;
}