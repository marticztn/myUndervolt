#define NOMINMAX
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <boost/filesystem.hpp>

#include "xOperation.h"
#include "xInterface.h"
#include "xUserVar.h"
#include "xTune.h"

using namespace std;
using namespace boost::filesystem;

xTune Tune;
const std::string DefaultPath = "C:\\Program Files (x86)\\Intel\\Intel(R) Extreme Tuning Utility\\Client";
const LPCWSTR ConsoleTitle = L"myUndervolt v1.2.2";
const LPCWSTR ExeTitle = L"myUndervolt.exe";
const LPCWSTR Service(L"XTU3SERVICE");

SHELLEXECUTEINFO shExeInfo = { 0 };
//Initialize structure here...

//xFileOp class member functions
xFileOp::xFileOp() {}
xFileOp::~xFileOp() {}

//xMisceOp class member functions
xMisceOp::xMisceOp() {}
xMisceOp::~xMisceOp() {}

void xMisceOp::CountDown(int Num)
{
	while (Num != 0)
	{
		if (_kbhit())
		{
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

			cout << endl << "Resetting data & Undeploying AutoMode..." << endl;

			xFileOp::opWriteFile(UserPath[3], 0);
			xFileOp::opWriteFile(UserPath[4], 0);

			for (int Path = 5; Path <= 8; ++Path)
				xFileOp::opWriteFile(UserPath[Path], 35);

			xFileOp::opWriteFile(UserPath[9], 45);
			xFileOp::opWriteFile(UserPath[10], 60);

			for (int Path = 11; Path < 14; ++Path)
				xFileOp::opWriteFile(UserPath[Path], 0);

			Sleep(1000);
			xInterface();
		}

		cout << Num << "..." << endl;
		Num--;

		Sleep(1000);
	}
}

int xFileOp::opReadFile(const char* FileName)
{
	int Status;
	std::ifstream inFile(FileName);

	if (!inFile.is_open())
	{
		cout << "Unable to open file (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	inFile >> Status;
	inFile.close();

	return Status;
}

void xFileOp::opWriteFile(const char* FileName, int Value)
{
	std::ofstream OutFile(FileName);

	if (!OutFile.is_open())
	{
		cout << "Unable to open file (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	OutFile << Value;
	OutFile.close();
}

void xFileOp::opWriteDirectory(string Directory)
{
	std::ofstream OutFile(UserPath[2]);

	if (!OutFile.is_open())
	{
		cout << "Unable to open file (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	OutFile << Directory;
	OutFile.close();
}

string xFileOp::opGetDirectory(void)
{
	string Dir;
	std::ifstream inFile(UserPath[2]);

	if (!inFile.is_open())
	{
		cout << "Unable to open file (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	getline(inFile, Dir);
	inFile.close();

	return Dir;
}

string xFileOp::opGetClipboardData(void)
{
	if (!OpenClipboard(nullptr))
	{
		cout << "Unable to open clipboard (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	HANDLE hClipData = GetClipboardData(CF_TEXT);

	if (hClipData == nullptr)
	{
		cout << "Unable to retrieve data from clipboard (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	char *Content = static_cast<char *>(GlobalLock(hClipData));
	if (Content == nullptr)
	{
		cout << "static_cast operation error (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	string ClipContent(Content);

	GlobalUnlock(hClipData);
	CloseHandle(hClipData);
	CloseClipboard();

	return ClipContent;
}

//xInitOp class member functions

xInitOp::xInitOp() {}
xInitOp::~xInitOp() {}

void xInitOp::opDetectRelatedProcess(void)
{
	int Count = 0;
	HANDLE hProcessSnap;
	PROCESSENTRY32 ProcEntry32;

	if ((hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
	{
		cout << "Cannot create tool help, error code: " << GetLastError() << endl;
		exit(EXIT_FAILURE);
	}

	//cout << "CreateToolhelp32Snapshot succeed..." << endl << endl;

	ProcEntry32.dwSize = sizeof(PROCESSENTRY32);

	//Test the first process, exit if failed
	if (!Process32First(hProcessSnap, &ProcEntry32))
	{
		cout << "Failed to retrieve the process information, error code: " << GetLastError() << endl;
		CloseHandle(hProcessSnap);
		exit(EXIT_FAILURE);
	}

	//system("pause");
	//putchar(10);

	int count = 0, arr = 0;
	DWORD TermPID[10] = { 0 };

	cout << "Current PID: " << GetCurrentProcessId() << endl;

	//If the previous operation succeed, enter this loop
	do
	{
		if (wcscmp(ProcEntry32.szExeFile, ExeTitle) == 0 && ProcEntry32.th32ProcessID != GetCurrentProcessId())
		{
			TermPID[arr] = ProcEntry32.th32ProcessID;
			wcout << ProcEntry32.szExeFile << " - " << ProcEntry32.th32ProcessID << endl;
			count++;
			arr++;
		}

	} while (Process32Next(hProcessSnap, &ProcEntry32));

	//Release the handle
	CloseHandle(hProcessSnap);

	//List all the related processes
	for (int i = 0; i < 10; ++i)
		cout << i + 1 << ". " << TermPID[i] << endl;

	HANDLE newHnd = NULL;
	DWORD currPID = GetCurrentProcessId();

	//Terminate the process in order
	for (arr = 0; arr < 10; ++arr)
	{
		if (TermPID[arr] != currPID && TermPID[arr] != 0)
		{
			newHnd = OpenProcess(PROCESS_ALL_ACCESS, NULL, TermPID[arr]);

			if (newHnd == NULL)
			{
				cout << "An error occurred when giving the privileges, Error Code: " << GetLastError() << endl;
				exit(EXIT_FAILURE);
			}

			if (TerminateProcess(newHnd, 0) == 0)
			{
				cout << "An error occurred when terminating the process, Error Code: " << GetLastError() << endl;
				exit(EXIT_FAILURE);
			}

			//Successfully terminated the process, set handle back to NULL
			else
				newHnd = NULL;
		}

		else
		{
			//cout << "No repetitive process detected..." << endl;
			break;
		}
	}

	//cout << endl << "Found " << count << " processes related to the target program" << endl;
}

void xInitOp::opInitializeWindow(void)
{
	HANDLE Handle;
	_SMALL_RECT rect;

	SetConsoleTitleW(ConsoleTitle);
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = 110;
	rect.Right = 36;

	if ((Handle = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
	{
		ShowWindow(xSysOp::opFindWindow(ConsoleTitle), SW_SHOW);
		ShowWindow(xSysOp::opFindWindow(ConsoleTitle), SW_SHOW);
		cout << "INVALID_HANDLE_VALUE (Error Code: " << GetLastError() << ")" << endl;
		exit(0);
	}

	SetConsoleWindowInfo(Handle, TRUE, &rect);

	//Should release the handle...
}

void xInitOp::opValidateFiles(void)
{
	system("cls");

	wcout << endl << L"\t\t\t\t" + AppVer + L" - Validating files and services" << endl << endl;
	cout << "\t\t\tPowered by Intel(R) Extreme Tunning Utility Command Line Tool" << endl << endl;

	bool isLost = false;

	for (int i = 0; i < 15; ++i)		//Should automatically detect how many paths are there
	{
		if (!exists(UserPath[i]))
		{
			cout << UserPath[i] << " does not exist..." << endl;
			isLost = true;
		}
	}

	if (isLost == true)
	{
		cout << endl << "Please make sure that the \"myUndervoltdata\" folder has been placed under C:\\, then try again..." << endl;
		system("pause");
		exit(0);
	}

	if (opReadFile(UserPath[3]) == 1)		//AutoMode
	{
		xInitOp::opEstablishService();

		//Add Native Windows 10 Toast Notification here

		cout << "AutoMode is enabled, press any key to cancel..." << endl << endl;
		xMisceOp::CountDown(5);
		cout << "Restoring and setting data..." << endl;

		ShowWindow(xSysOp::opFindWindow(ConsoleTitle), SW_MINIMIZE);
		//ShowWindow(xSysOp::opFindWindow(ConsoleTitle), SW_MINIMIZE);

		for (int i = 11; i < 14; ++i)
		{
			if (opReadFile(UserPath[i]) == 1 && i == 11)		//Core Voltage Offset section
				xSysOp::ExecuteCommand(shExeInfo, 't', 34, xFileOp::opReadFile(UserPath[4]));

			else if (opReadFile(UserPath[i]) == 1 && i == 12)	//Core Frequency section
			{
				for (int ACS = 29, Path = 5; ACS <= 32, Path <= 8; ++ACS, ++Path)
					xSysOp::ExecuteCommand(shExeInfo, 't', ACS, xFileOp::opReadFile(UserPath[Path]));
			}

			else if (opReadFile(UserPath[i]) == 1 && i == 13)	//Power section
			{
				for (int POW = 48, Path = 9; POW > 46, Path < 11; --POW, ++Path)
					xSysOp::ExecuteCommand(shExeInfo, 't', POW, xFileOp::opReadFile(UserPath[Path]));
			}
		}

		xSysOp::opSleepDetection();
	}

	else if (opReadFile(UserPath[3]) == 0)	//ConfigMode - No AutoMode enabled
	{
		if (opReadFile(UserPath[1]) == 0)	//Use default path
		{
			path Path(DefaultPath + "\\XtuCLI.exe");
			if (!exists(Path))
			{
				string cusPath, cusPathForExe;

				cout << "Current Path: " << opGetDirectory() << endl << endl;
				cout << "Cannot find required file (XtuCLI.exe), consider re-installing XTU" << endl;
				cout << "Or you can specify your own XtuCLI.exe installation path here (copy the path, then press enter here)" << endl << endl;
				cout << "Waiting...(Close this window abort)" << endl;

				system("pause");

				cusPath = opGetClipboardData();
				cusPathForExe = cusPath;

				for (unsigned int i = 0; i < cusPath.length(); ++i)
				{
					if (cusPath[i] == '\\')
					{
						cusPath.insert(i, "\\");
						++i;
					}
				}

				cusPath.append("\\\\XtuCLI.exe");

				//cout << "cusPath: " << cusPath << endl;
				//system("pause");

				cout << endl << "Path copied: " << cusPath << endl;
				cout << "Checking required file..." << endl << endl;

				//system("pause");
				path userPath(cusPath);
				if (!exists(userPath))
				{
					cout << "Cannot find required file (XtuCLI.exe), consider re-installing XTU or try again" << endl;
					opWriteFile(UserPath[1], 0);
					opWriteDirectory(DefaultPath);
					Sleep(4000);
					opValidateFiles();
				}
				else
				{
					opWriteFile(UserPath[1], 1);
					opWriteDirectory(cusPath);

					cout << "Found, user specified path saved" << endl;
				}

				Sleep(1000);
			}
			else
			{
				opWriteFile(UserPath[1], 0);
				opWriteDirectory(DefaultPath);
			}
		}
		else   //Use user specified path
		{
			string usrPath = opGetDirectory();

			path Path(usrPath);
			if (!exists(Path))
			{
				cout << "Cannot find required file (XtuCLI.exe), consider re-installing XTU or try again" << endl;
				cout << "Trying to find files under default path..." << endl;
				opWriteFile(UserPath[1], 0);
				opWriteDirectory(DefaultPath);
				Sleep(2000);
				opValidateFiles();
			}
			else
				cout << "Required file found..." << endl;
		}
	}
	else
	{
		cout << "Value is not valid, AutoMode disabled..." << endl;
		opWriteFile(UserPath[3], 0);
	}

	Sleep(1000);
}

void xInitOp::opEstablishService(void)
{
	SC_HANDLE scHandle, opsHandle;

	scHandle = (OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CONNECT));
	if (scHandle == NULL)
	{
		cout << "An error occurred while connecting to the Service Control Manager (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Successfully connected to the Service Control Manager..." << endl;

	opsHandle = OpenServiceW(scHandle, Service, SERVICE_QUERY_STATUS | SERVICE_START);
	if (opsHandle == NULL)
	{
		cout << "An error occured while opening the target service...(Error code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "Successfully opened target service..." << endl;
	CloseServiceHandle(scHandle);

	DWORD BytesNeeded = 0;
	SERVICE_STATUS_PROCESS svStatPro;
	if (QueryServiceStatusEx(opsHandle, SC_STATUS_PROCESS_INFO, (BYTE*)&svStatPro, sizeof(svStatPro), &BytesNeeded) == 0)
	{
		cout << "Cannot get target service status (Error Code: " << GetLastError() << ")" << endl;
		exit(EXIT_FAILURE);
	}

	putchar(10);

	if (svStatPro.dwCurrentState == SERVICE_STOPPED || svStatPro.dwCurrentState == SERVICE_PAUSED)
	{
		cout << "Staring required service..." << endl;
		if (StartServiceW(opsHandle, 0, NULL) == 0)
		{
			cout << "Cannot start the target service (Error Code: " << GetLastError() << ")" << endl;
			exit(EXIT_FAILURE);
		}
		cout << "Target service is now running..." << endl;
	}
	else if (svStatPro.dwCurrentState == SERVICE_RUNNING)
		cout << "The target service is already running, skipping..." << endl << endl;

	CloseServiceHandle(opsHandle);
}

//xSysOp class member functions

xSysOp::xSysOp() {};
xSysOp::~xSysOp() {};

void xSysOp::opClean(void)
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.sync();
}

HWND xSysOp::opFindWindow(LPCTSTR lpWindowName)
{
	HWND Window;
	if ((Window = FindWindow(NULL, lpWindowName)) != NULL)
		return Window;
	else
		return NULL;
}

void xSysOp::opSleepDetection(void)
{
	//Assign window info
	WNDCLASS WindowClass = { 0 };
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.lpfnWndProc = opSleepDetectionProc;
	WindowClass.lpszClassName = (L"Sleep detection");

	//Register window
	if (!GetClassInfo(WindowClass.hInstance, WindowClass.lpszClassName, &WindowClass))
	{
		if (!RegisterClass(&WindowClass))
		{
			cout << "Cannot register class (Error Code: " << GetLastError() << ")" << endl;
			exit(EXIT_FAILURE);
		}
	}

	//Create hidden window
	HWND WinHandle = NULL;
	if (!FindWindow(WindowClass.lpszClassName, ConsoleTitle))
	{
		if ((WinHandle = CreateWindow(WindowClass.lpszClassName, L"", 0, 0, 0, 0, 0, NULL, NULL, NULL, 0)) == NULL)
		{
			cout << "Cannot create window (Error Code: " << GetLastError() << ")" << endl;
			exit(EXIT_FAILURE);
		}
	}

	int upRes;
	MSG Message;

	//Receive window messages
	while (upRes = GetMessage(&Message, WinHandle, 0, 0))
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		if (upRes == -1)
		{
			ShowWindow(opFindWindow(ConsoleTitle), SW_SHOW);
			cout << "Error when getting window messages (Error Code: " << GetLastError() << endl;
			DestroyWindow(WinHandle);
			exit(EXIT_FAILURE);
		}

		else
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
			//Prob need to wait for a sec?
		}
	}
}

void xSysOp::opWait(HANDLE hHandle)
{
	if (WaitForSingleObject(hHandle, INFINITE) != WAIT_OBJECT_0)
	{
		cout << "An unkown error occured" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}

//Example usage: xSysOp::ExecuteCommand(shExeInfo, 't', 34, 0);
string xSysOp::ExecuteCommand(SHELLEXECUTEINFO shExeInfo, char ExeType, unsigned long ID, int Value)
{
	string Parameter = "";

	if (ExeType == 't' && ID == 34)			//An exception for Core Voltage Offset
		Parameter = " -t -id 34 -v 0";
	else if (ExeType == 't')				//Tunning mode
		Parameter = " -t -id " + to_string(ID) + " -v " + to_string(Value);
	if (ExeType == 'm')						//Monitoring mode
		Parameter = " -m -id " + to_string(ID);

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

	if (ExeType == 't')
	{
		switch (ID)
		{
		case 34:	//Core Voltage Offset
		{
			ShellExecuteEx(&shExeInfo);	//Set to 0 first
			xSysOp::opWait(shExeInfo.hProcess);

			if (Value == 0)
				break;

			Parameter = " -t -id 34 -v " + to_string(Value);
			wstring wParameter = wstring(Parameter.begin(), Parameter.end());
			shExeInfo.lpParameters = wParameter.c_str();

			ShellExecuteEx(&shExeInfo);	//Set to user defined value
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
		}

			return "";

		case 29:	//1 Active Core
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		case 30:	//2 Active Cores
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		case 31:	//3 Active Cores
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		case 32:	//4 Active Cores
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		case 47:	//Turbo Boost Short Power Max
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		case 48:	//Turbo Boost Power Max
			ShellExecuteEx(&shExeInfo);
			xSysOp::opWait(shExeInfo.hProcess);
			CloseHandle(shExeInfo.hProcess);
			break;

		default:
			break;
		}
	}

	return string();
}

LRESULT _stdcall xSysOp::opSleepDetectionProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HANDLE handle;
	WORD currentConsoleAttribute;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (GetConsoleScreenBufferInfo(handle, &csbi))
		currentConsoleAttribute = csbi.wAttributes;

	//Prob need to check if there's any error inside opSleepDetection when registering window class
	if (uMsg == WM_POWERBROADCAST)
	{
		if (wParam == PBT_APMRESUMESUSPEND)
		{
			ShowWindow(opFindWindow(ConsoleTitle), SW_SHOW);
			system("cls");
			wcout << endl << L"\t\t\t\t\t" + AppVer + L" - Resuming" << endl << endl;
			cout << "\t\t\tPowered by Intel(R) Extreme Tunning Utility Command Line Tool" << endl << endl;

			xInitOp::opEstablishService();
			
			SetConsoleTextAttribute(handle, 14);
			cout << "System restored, press any key to undeploy AutoMode & reset data..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);

			xMisceOp::CountDown(5);

			SetConsoleTextAttribute(handle, 14);
			cout << endl << "Restoring and setting data..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);

			ShowWindow(xSysOp::opFindWindow(ConsoleTitle), SW_MINIMIZE);

			for (int i = 11; i < 14; ++i)
			{
				if (xFileOp::opReadFile(UserPath[i]) == 1 && i == 11)		//Core Voltage Offset section
					xSysOp::ExecuteCommand(shExeInfo, 't', 34, xFileOp::opReadFile(UserPath[4]));

				else if (xFileOp::opReadFile(UserPath[i]) == 1 && i == 12)	//Core Frequency section
				{
					for (int ACS = 29, Path = 5; ACS <= 32, Path <= 8; ++ACS, ++Path)
						xSysOp::ExecuteCommand(shExeInfo, 't', ACS, xFileOp::opReadFile(UserPath[Path]));
				}

				else if (xFileOp::opReadFile(UserPath[i]) == 1 && i == 13)	//Power section
				{
					for (int POW = 48, Path = 9; POW < 46, Path < 11; --POW, ++Path)
						xSysOp::ExecuteCommand(shExeInfo, 't', POW, xFileOp::opReadFile(UserPath[Path]));
				}
			}

			opSleepDetection();
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}