#using <System.dll>

#include "TMT.h"
#include "LiDAR.h"
#include "GNSS.h"



//set up thread memory
error_state ThreadManagement::setupShareMemory()
{
	TMT_SM_TM_ = gcnew SM_ThreadManagement;
	TMT_SM_LiDAR_ = gcnew SM_LiDAR;
	TMT_SM_GNSS_ = gcnew SM_GNSS;

	return error_state::SUCCESS;
}

//thread fuction
void ThreadManagement::threadFunction()
{
	//int i = 0;
	Console::WriteLine("TMT Thread is starting");

		//make a list of thread properties
	ThreadPropertiesList = gcnew array<ThreadProperties^>{
		gcnew ThreadProperties(gcnew ThreadStart(gcnew LiDAR(TMT_SM_TM_, TMT_SM_LiDAR_), &LiDAR::threadFunction), true, bit_LiDAR, "LiDAR Thread"),
		gcnew ThreadProperties(gcnew ThreadStart(gcnew GNSS(TMT_SM_TM_, TMT_SM_GNSS_), &GNSS::threadFunction), false, bit_GNSS, "GNSS Thread"),
	};

	//make a list of thread
	ThreadList = gcnew array<Thread^>(ThreadPropertiesList->Length);

	//make the stopwatch list
	TMT_StopwatchList = gcnew array<Stopwatch^>(ThreadPropertiesList->Length);

	//make thread barriers
	TMT_SM_TM_->ThreadBarrier = gcnew Barrier(ThreadPropertiesList->Length + 1);

	//start all the threads
	for (int i = 0; i < ThreadPropertiesList->Length; i++)
	{
		TMT_StopwatchList[i] = gcnew Stopwatch;
		ThreadList[i] = gcnew Thread(ThreadPropertiesList[i]->ThreadStart_);//ThreadPropertiesList[0]->ThreadStart_ = ThreadStart(gcnew LiDAR(), &LiDAR::threadFunction)
		ThreadList[i]->Start();
	}

	//wait at the TMT thread barrier
	TMT_SM_TM_->ThreadBarrier->SignalAndWait();

	//start all the stopwatches
	for (int i = 0; i < ThreadList->Length; i++)
	{
		TMT_StopwatchList[i]->Start();
	}

	//start the thread loop + keep checking the heartbeats
	while (!Console::KeyAvailable && !getShutdownFlag())
	{
		Console::WriteLine("TMT Thread is running");
		processHeartbeats();//keep checking the heartbeats
		Thread::Sleep(50);

		//if (i++ > 50) break;
	}

	//end the thread loop + shut down all threads
	shutdownThreads();

	//join all threads
	for (int i = 0; i < ThreadPropertiesList->Length; i++)
	{
		ThreadList[i]->Join();
	}

	Console::WriteLine("TMT Thread is terminated");
}

//process heartbeat
error_state ThreadManagement::processHeartbeats()
{
	//check the heartbeat flag of the ith thread (is it high?)
	for (int i = 0; i < ThreadList->Length; i++)
	{
		if (TMT_SM_TM_->heartbeat & ThreadPropertiesList[i]->BitID)
		{
			//if high, put the ith bit (flag) down
			TMT_SM_TM_->heartbeat &= ~ThreadPropertiesList[i]->BitID;
			//reset the stopwatch
			TMT_StopwatchList[i]->Restart();
		}

		else
		{
			//check the stopwatch, is the time exceed the crash limit?
			if (TMT_StopwatchList[i]->ElapsedMilliseconds > CRASH_LIMIT)
			{
				//is the ith thread a critical one?
				if (ThreadPropertiesList[i]->Critical)
				{
					Console::WriteLine(ThreadPropertiesList[i]->ThreadName + " (critical) failure. Shutting down all threads");
					shutdownThreads();//shut down all
					return error_state::ERR_CRITICAL_THREAD_FAILURE;
				}

				else
				{
					Console::WriteLine(ThreadPropertiesList[i]->ThreadName + " failed...Restarting");
					ThreadList[i]->Abort();
					ThreadList[i] = gcnew Thread(ThreadPropertiesList[i]->ThreadStart_);
					TMT_SM_TM_->ThreadBarrier = gcnew Barrier(1);
					ThreadList[i]->Start();//try to restart
				}
			}
		}
	}

	return error_state::SUCCESS;	
}

//shut down threads
void ThreadManagement::shutdownThreads()
{
	TMT_SM_TM_->shutdown = 0xFF; //0b1111 1111
}

//get shutdown flag
bool ThreadManagement::getShutdownFlag()
{
	return (TMT_SM_TM_->shutdown & bit_TM);
}