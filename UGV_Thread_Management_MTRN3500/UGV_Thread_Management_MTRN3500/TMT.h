#pragma once
#using <System.dll>
#include "SMObjects.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

ref struct ThreadProperties
{
	ThreadStart^ ThreadStart_;
	bool Critical;
	String^ ThreadName;
	uint8_t BitID;

	ThreadProperties(ThreadStart^ start, bool critical, uint8_t bit_id, String^ threadname)
	{
		ThreadStart_ = start;
		Critical = critical;
		BitID = bit_id;
		ThreadName = threadname;
	}
};

ref class ThreadManagement
{
public:
	//set up thread memory
	error_state setupShareMemory();

	//thread fuction
	void threadFunction();

	//process heartbeat
	error_state processHeartbeats();

	//shut down threads
	void shutdownThreads();

	//get shutdown flag
	bool getShutdownFlag();

private:
	SM_ThreadManagement^ TMT_SM_TM_;
	SM_LiDAR^ TMT_SM_LiDAR_;
	SM_GNSS^ TMT_SM_GNSS_;
	array<Stopwatch^>^ TMT_StopwatchList;
	array<Thread^>^ ThreadList;
	array<ThreadProperties^>^ ThreadPropertiesList;
};