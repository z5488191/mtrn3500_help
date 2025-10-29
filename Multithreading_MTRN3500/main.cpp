#using <System.dll>

using namespace System;
using namespace System::Threading;

ref class Mutexes
{
public:
    static Mutex^ ourMutex = gcnew Mutex;
    //static Object^ lockObject = gcnew Object();
    static Barrier^ threadBarrier = gcnew Barrier(3);
    static bool Quit = false;

public:
    static void LiDAR()
    {
        Console::WriteLine("LiDAR Thread Started");
        Thread::Sleep(5000);
        Console::WriteLine("LiDAR Thread Ready");
        threadBarrier->SignalAndWait();
        while (!Quit)
        {
            CriticalSection();
            Console::WriteLine("LiDAR Scan Completed\n");
            Thread::Sleep(50);
        }
    }

    static void GNSS()
    {
        Console::WriteLine("GNSS Thread Started");
        Thread::Sleep(10000);
        Console::WriteLine("GNSS Thread Ready");
        threadBarrier->SignalAndWait();
        while (!Quit)
        {
            CriticalSection();
            Console::WriteLine("GNSS Scan Completed\n");
            Thread::Sleep(50);
        }
    }

    static void DriveControl()
    {
        Console::WriteLine("DriveControl Thread Started");
        Thread::Sleep(15000);
        Console::WriteLine("DriveControl Thread Ready\n");
        threadBarrier->SignalAndWait();
        while (!Quit)
        {
            CriticalSection();
            Console::WriteLine("DriveControl Scan Completed\n");
            Thread::Sleep(50);
        }
    }

    static void CriticalSection()
    {
        ourMutex->WaitOne();
        String^ name = Thread::CurrentThread->Name;
        Console::WriteLine(name + " is requesting the mutex");
        //ourMutex->WaitOne();
        //Monitor::Enter(lockObject);
        Console::WriteLine(name + " is in critical section");
        Thread::Sleep(200);
        //Monitor::Exit(lockObject);
        ourMutex->ReleaseMutex();
        Console::WriteLine(name + " has released the nutex");
    }

};

int main()
{
    Thread^ LiDARThread = gcnew Thread(gcnew ThreadStart(&Mutexes::LiDAR));
    LiDARThread->Name = "LiDAR Thread";

    Thread^ GNSSThread = gcnew Thread(gcnew ThreadStart(&Mutexes::GNSS));
    GNSSThread->Name = "GNSS Thread";

    Thread^ DriveControlThread = gcnew Thread(gcnew ThreadStart(&Mutexes::DriveControl));
    DriveControlThread->Name = "DriveControl Thread";

    LiDARThread->Start();
    GNSSThread->Start();
    DriveControlThread->Start();

    Console::ReadKey();
    Mutexes::Quit = true;
    Console::ReadKey();
    return 0;
}