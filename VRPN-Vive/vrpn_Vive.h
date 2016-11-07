#pragma once

<<<<<<< HEAD
#include "vrpn_Tracker.h"
#include "vrpn_Analog.h"
#include "vrpn_Button.h"
#include "vrpn_Connection.h"
=======
#include "vrpn/vrpn_Tracker.h"
#include "vrpn/vrpn_Analog.h"
#include "vrpn/vrpn_Button.h"
#include "vrpn/vrpn_Connection.h"
>>>>>>> e8db19a04c2ef1c6612748b9a47ac5c65940206f

class vrpn_Vive:
	public vrpn_Tracker,
	public vrpn_Analog,
	public vrpn_Button
{
public:
	vrpn_Vive(const char* name, vrpn_Connection* cxn);
	virtual ~vrpn_Vive();
private:
	vrpn_Vive();
	vrpn_Vive(const vrpn_Vive&);
	const vrpn_Vive& operator=(const vrpn_Vive&);
};

