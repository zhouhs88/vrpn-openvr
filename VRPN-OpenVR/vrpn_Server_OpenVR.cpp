#include <iostream>
#include <string>
#include <quat.h>
#include <vrpn_Connection.h>
#include "vrpn_Server_OpenVR.h"

vrpn_Server_OpenVR::vrpn_Server_OpenVR() {
    // Initialize OpenVR
    vr::EVRInitError eError = vr::VRInitError_None;
    vr = std::unique_ptr<vr::IVRSystem>(vr::VR_Init(&eError, vr::VRApplication_Overlay));
    if (eError != vr::VRInitError_None) {
        vr.reset(nullptr);
        std::cerr << "Unable to init VR runtime: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
    }

    // Initialize VRPN Connection
    std::string connectionName = ":" + std::to_string(vrpn_DEFAULT_LISTEN_PORT_NO);
    connection = vrpn_create_server_connection(connectionName.c_str());
}


vrpn_Server_OpenVR::~vrpn_Server_OpenVR() {
    vr::VR_Shutdown();
    if (connection) {
        connection->removeReference();
        connection = NULL;
    }
}

void vrpn_Server_OpenVR::mainloop() {
    // Get Tracking Information
    vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    vr->GetDeviceToAbsoluteTrackingPose(
        vr::TrackingUniverseStanding,
        0/*float fPredictedSecondsToPhotonsFromNow*/,
        m_rTrackedDevicePose,
        vr::k_unMaxTrackedDeviceCount
    );

    for (vr::TrackedDeviceIndex_t unTrackedDevice = 0;
         unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++) {
        // Forget about disconnected devices
        if (!m_rTrackedDevicePose[unTrackedDevice].bDeviceIsConnected) {
            continue;
        }

        // Treat different device types separately
        switch (vr->GetTrackedDeviceClass(unTrackedDevice)) {
            case vr::TrackedDeviceClass_HMD: {
                vrpn_Tracker_OpenVR_HMD *hmd{nullptr};
                auto search = hmds.find(unTrackedDevice);
                if (search == hmds.end()) {
                    std::unique_ptr<vrpn_Tracker_OpenVR_HMD> newHMD = std::make_unique<vrpn_Tracker_OpenVR_HMD>(
                        "openvr/hmd/" + std::to_string(unTrackedDevice),
                        connection,
                        vr.get()
                    );
                    hmd = newHMD.get();
                    hmds[unTrackedDevice] = std::move(newHMD);
                } else {
                    hmd = search->second.get();
                }
                hmd->updateTracking(&m_rTrackedDevicePose[unTrackedDevice]);
                hmd->mainloop();
                break;
            }
            case vr::TrackedDeviceClass_Controller: {
                vrpn_Tracker_OpenVR_Controller *controller{nullptr};
                auto search = controllers.find(unTrackedDevice);
                if (search == controllers.end()) {
                    std::unique_ptr<vrpn_Tracker_OpenVR_Controller> newController = std::make_unique<vrpn_Tracker_OpenVR_Controller>(
                        "openvr/controller/" + std::to_string(unTrackedDevice),
                        connection,
                        vr.get()
                    );
                    controller = newController.get();
                    controllers[unTrackedDevice] = std::move(newController);
                } else {
                    controller = search->second.get();
                }
                controller->updateTracking(&m_rTrackedDevicePose[unTrackedDevice]);
                controller->updateController(unTrackedDevice);
                controller->mainloop();
                break;
            }
            default: {
                break;
            }
        }
    }

    // Send and receive all messages.
    connection->mainloop();

    // Bail if the connection is in trouble.
    if (!connection->doing_okay()) {
        std::cerr << "Connection is not doing ok. Should we bail?" << std::endl;
    }
}

