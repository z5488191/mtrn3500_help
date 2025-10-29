#using <System.dll>

using namespace System;

int main()
{
	int i = 3500;
	double pi = 3.1415926;

	//Console::Write("MTRN3500, execution completed!");
	Console::WriteLine("MTRN3500, execution completed!");

	//Colour control
	Console::ForegroundColor = ConsoleColor::Red;
	//Console::BackgroundColor = ConsoleColor::Green;

	//Cursor control
	do
	{
		Console::SetCursorPosition(10, 10);
		Console::WriteLine("{0, 15:D} {1, 15:F3}", i++, pi++);
	} while (!Console::KeyAvailable);

	//Format printing
	//Console::WriteLine("{0, 15:D} {1, 15:F3}", i, pi);

	Console::ReadKey();
	Console::ReadKey();
	return 0;
}