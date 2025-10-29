#using <System.dll>
#include "LiDAR.h"

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;

LiDAR::LiDAR(SM_ThreadManagement^ SM_TM, SM_LiDAR^ SM_lidar)
{
	LiDAR_SM_TM_ = SM_TM;
	LiDAR_SM_LiDAR_ = SM_lidar;
	LiDAR_Stopwatch = gcnew Stopwatch;
}

error_state LiDAR::setupShareMemory()
{
	return error_state::SUCCESS;
}

void LiDAR::threadFunction()
{
	//int i = 0;
	Console::WriteLine("LiDAR Thread is starting");

	//wait for the barrier
	LiDAR_SM_TM_->ThreadBarrier->SignalAndWait();

	//start the stopwatch
	LiDAR_Stopwatch->Start();

	//start the thread loop
	while (!getShutdownFlag())
	{
		Console::WriteLine("LiDAR Thread is running");
		processHeartbeats();
		if (communicate() == SUCCESS && checkData() == SUCCESS)
		{
			processSharedMemory();//process the data
		}
		Thread::Sleep(20);

		//if (i++ > 50) break;
	}

	Console::WriteLine("LiDAR Thread is terminated");		
}

error_state LiDAR::processHeartbeats()
{
	//check if the LiDAR bit in the hearbeat byte is low
	if ((LiDAR_SM_TM_->heartbeat & bit_LiDAR) == 0)
	{
		//put the LiDAR bit high
		LiDAR_SM_TM_->heartbeat |= bit_LiDAR;
		//reset the stopwatch
		LiDAR_Stopwatch->Restart();
	}

	else
	{
		//check if the time elasped exceed the crash time limit
		if (LiDAR_Stopwatch->ElapsedMilliseconds > CRASH_LIMIT)
		{
			//shut down all threads
			shutdownThreads();
			return error_state::ERR_TMT_FAILURE;
		}
	}
	
	return error_state::SUCCESS;
}

void LiDAR::shutdownThreads()
{
	LiDAR_SM_TM_->shutdown = 0xFF;
}
bool LiDAR::getShutdownFlag()
{
	return LiDAR_SM_TM_->shutdown & bit_LiDAR;
}

error_state LiDAR::communicate()
{
	return error_state::SUCCESS;
}

error_state LiDAR::checkData()
{
	return error_state::SUCCESS;
}

error_state LiDAR::processSharedMemory()
{
	return error_state::SUCCESS;
}