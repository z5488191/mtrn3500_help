#pragma once
#include <cstdint>
#using <System.dll>
using namespace System::Threading;

#define LiDAR_DATA_LENGTH 361
#define CRASH_LIMIT 1000

#define bit_TM 0b00000001
#define bit_LiDAR 0b00000010
#define bit_GNSS 0b00000100

ref class SM_ThreadManagement
{
public:
	uint8_t heartbeat;
	uint8_t shutdown;
	Barrier^ ThreadBarrier;
};

ref class SM_LiDAR
{
public:
	array<double>^ x;
	array<double>^ y;

	SM_LiDAR()
	{
		x = gcnew array<double>(LiDAR_DATA_LENGTH);
		y = gcnew array<double>(LiDAR_DATA_LENGTH);
	}
};

ref class SM_GNSS
{
public:
	double Easting;
	double Northing;
	double Altitude;
};

enum error_state
{
	SUCCESS,
	ERR_SM,
	ERR_CRITICAL_THREAD_FAILURE,
	ERR_NONCRITICAL_THREAD_FAILURE,
	ERR_TMT_FAILURE,
};