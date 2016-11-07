#pragma once

#include "vrpn/vrpn_Tracker.h"
#include "vrpn/vrpn_Analog.h"
#include "vrpn/vrpn_Button.h"
#include "vrpn/vrpn_Connection.h"

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

