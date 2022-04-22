#include <iostream>

#include "xTune.h"
#include "xOperation.h"
#include "xUserVar.h"

using namespace std;

xTune::xTune() {};
xTune::~xTune() {};

void xTune::CoreVoltageOffset(int Value)
{
	SHELLEXECUTEINFO shExeInfo = { 0 };

	string Parameter = " -t -id 34 -v 0";
	string Directory = xFileOp::opGetDirectory();
	wstring wParameter = wstring(Parameter.begin(), Parameter.end());
	wstring wDirectory = wstring(Directory.begin(), Directory.end());

	shExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExeInfo.hwnd = NULL;
	shExeInfo.lpVerb = L"open";
	shExeInfo.lpFile = L"XtuCLI.exe";
	shExeInfo.lpParameters = wParameter.c_str();
	shExeInfo.lpDirectory = wDirectory.c_str();
	shExeInfo.nShow = SW_SHOW;
	shExeInfo.hInstApp = NULL;

	ShellExecuteEx(&shExeInfo);	//Set to 0V first
	if (WaitForSingleObject(shExeInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "An unkown error occured" << endl;
		exit(EXIT_FAILURE);
	}

	if (Value == 0)
		Parameter = " -t -id 34 -v 0";// +to_string(xFileOp::opReadFile(UserPath[4]));
	else
		Parameter = " -t -id 34 -v " + to_string(Value);

	wParameter = wstring(Parameter.begin(), Parameter.end());
	shExeInfo.lpParameters = wParameter.c_str();

	ShellExecuteEx(&shExeInfo);	//Now set to user defined value
	if (WaitForSingleObject(shExeInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "An unkown error occured" << endl;
		exit(EXIT_FAILURE);
	}

	TerminateProcess(shExeInfo.hProcess, 1);
	CloseHandle(shExeInfo.hProcess);
}

void xTune::ProcessorCoreRatio(int Value)
{
	string Parameter = " -t -id 3489660933 -v 30";
	string Directory = xFileOp::opGetDirectory();
	wstring wParameter = wstring(Parameter.begin(), Parameter.end());
	wstring wDirectory = wstring(Directory.begin(), Directory.end());

	SHELLEXECUTEINFO shExeInfo = { 0 };
	shExeInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExeInfo.hwnd = NULL;
	shExeInfo.lpVerb = L"open";
	shExeInfo.lpFile = L"XtuCLI.exe";
	shExeInfo.lpParameters = wParameter.c_str();
	shExeInfo.lpDirectory = wDirectory.c_str();
	shExeInfo.nShow = SW_SHOW;
	shExeInfo.hInstApp = NULL;

	/* Prob need to check the PCR value 
	ShellExecuteEx(&shExeInfo);	//Set to 3.0 Ghz first
	if (WaitForSingleObject(shExeInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "An unkown error occured" << endl;
		exit(EXIT_FAILURE);
	}
	*/

	if (Value == 0)
		Parameter = " -t -id 3489660933 -v " + to_string(xFileOp::opReadFile(UserPath[5]));
	else
		Parameter = " -t -id 3489660933 -v " + to_string(Value);

	wParameter = wstring(Parameter.begin(), Parameter.end());
	shExeInfo.lpParameters = wParameter.c_str();

	ShellExecuteEx(&shExeInfo);	//Now set to user defined value
	if (WaitForSingleObject(shExeInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "An unkown error occured" << endl;
		exit(EXIT_FAILURE);
	}

	TerminateProcess(shExeInfo.hProcess, 1);
	CloseHandle(shExeInfo.hProcess);
}