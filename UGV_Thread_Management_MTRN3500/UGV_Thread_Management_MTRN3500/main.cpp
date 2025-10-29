#include "TMT.h"

int main()
{
	ThreadManagement^ myTMT = gcnew ThreadManagement();

	myTMT->setupShareMemory();

	myTMT->threadFunction();

	Console::ReadKey();
	Console::ReadKey();

	return 0;
}