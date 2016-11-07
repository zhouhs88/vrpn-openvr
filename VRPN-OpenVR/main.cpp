#include <windows.h>
#include <iostream>
#include <openvr.h>
#include <string>
#include <memory>
#include "vrpn_Connection.h"
#include "vrpn_Tracker_OpenVR.h"

static volatile int done = 0;
vrpn_Connection *connection;
std::unique_ptr<vrpn_Tracker_OpenVR> openvr{};

// install a signal handler to shut down the devices
// On Windows, the signal handler is run in a different thread from
// the main application.  We don't want to go destroying things in
// here while they are being used there, so we set a flag telling the
// main program it is time to exit.
#if defined(_WIN32) && !defined(__CYGWIN__)
/**
* Handle exiting cleanly when we get ^C or other signals.
*/
BOOL WINAPI handleConsoleSignalsWin(DWORD signaltype)
{
	switch (signaltype) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		done = 1;
		return TRUE;
		// Don't exit, but return FALSE so default handler
		// gets called. The default handler, ExitProcess, will exit.
	default:
		return FALSE;
	}
}

#endif

void shutDown() {
	if (connection) {
		connection->removeReference();
		connection = NULL;
	}
}

int main(int argc, char *argv[]) {
	std::string connectionName = ":" + std::to_string(vrpn_DEFAULT_LISTEN_PORT_NO);
	connection = vrpn_create_server_connection(connectionName.c_str());
	openvr = std::make_unique<vrpn_Tracker_OpenVR>("openvr", connection);

	while (!done) {
		// Let the generic object server do its thing.
		if (openvr) {
			openvr->mainloop();
		}

		// Send and receive all messages.
		connection->mainloop();

		// Bail if the connection is in trouble.
		if (!connection->doing_okay()) {
			shutDown();
		}

		vrpn_SleepMsecs(0);
	}

	shutDown();

    return 0;
}