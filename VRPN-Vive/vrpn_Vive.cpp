#include "vrpn_Vive.h"

vrpn_Vive::vrpn_Vive(const char* name, vrpn_Connection * cxn) :
	vrpn_Tracker(name, cxn),
	vrpn_Analog(name,cxn),
	vrpn_Button(name,cxn)
{
}


vrpn_Vive::~vrpn_Vive()
{
}
