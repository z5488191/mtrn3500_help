#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref class LiDAR
{
public:
	LiDAR(SM_ThreadManagement^ SM_TM, SM_LiDAR^ SM_lidar);

	error_state setupShareMemory();
	void threadFunction();
	error_state processHeartbeats();
	void shutdownThreads();
	bool getShutdownFlag();
	error_state communicate();
	error_state checkData();
	error_state processSharedMemory();

private:
	SM_ThreadManagement^ LiDAR_SM_TM_;
	SM_LiDAR^ LiDAR_SM_LiDAR_;
	Stopwatch^ LiDAR_Stopwatch;
};