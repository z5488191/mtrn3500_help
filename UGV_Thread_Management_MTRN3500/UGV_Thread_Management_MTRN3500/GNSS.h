#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref class GNSS
{
public:
	GNSS(SM_ThreadManagement^ SM_TM, SM_GNSS^ SM_GNSS);

	error_state setupShareMemory();
	void threadFunction();
	error_state processHeartbeats();
	void shutdownThreads();
	bool getShutdownFlag();
	error_state communicate();
	error_state checkData();
	error_state processSharedMemory();

private:
	SM_ThreadManagement^ GNSS_SM_TM_;
	SM_GNSS^ GNSS_SM_GNSS_;
	Stopwatch^ GNSS_Stopwatch;
};