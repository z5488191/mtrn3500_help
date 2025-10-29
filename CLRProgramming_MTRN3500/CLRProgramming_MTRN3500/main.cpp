#using <System.dll>

using namespace System;

ref class Data
{
	double A;
	double B;
	double C;
public:
	Data(double a, double b, double c) : A(a), B(b), C(c) {}
	double Get_A() { return A; }
	double Get_B() { return B; }
	double Get_C() { return C; }
};

int main()
{
	//Instantiate a Data object
	Data^ DataHndl = gcnew Data(2, 3, 4);//on the managed heap

	Data MyData(5, 6, 7); //allocated on heap
	//Exceptions for stack semantics: Strings and Arrays. 

	Data^ MyDataHndl = % MyData;
	
	//int a = 3500; //allocate on stack
	//double pi = 3.1415926; //allocate on stack
	//double vec[1000000];

	int^ a = gcnew int(3500);//on the managed heap
	double^ b = gcnew double(3.1415926); //on the managed heap

	Console::WriteLine("{0, 15:D} {1, 15:F3}", a, b);
	Console::WriteLine("{0, 15:F3}", DataHndl->Get_A());
	Console::WriteLine("{0, 15:F3}", MyDataHndl->Get_A());

	Console::ReadKey();
	return 0;
}