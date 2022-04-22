#include <iostream>
#include <string>
#include <conio.h>

#include "xInterface.h"
#include "xOperation.h"
#include "xUserVar.h"
#include "xTune.h"

using namespace std;

const std::wstring AppVer(ConsoleTitle);
const char* UserPath[] =
{
	//Required file path: folderPath, statusPath, userDirPath, starUpPath (0~3)
	"C:\\myUndervoltdata",										//0
	"C:\\myUndervoltdata\\Status.dat",							//1
	"C:\\myUndervoltdata\\UserDir.dat",							//2
	"C:\\myUndervoltdata\\Startup.txt",							//3

	//UserData file path (4~9)
	"C:\\myUndervoltdata\\UserData\\CVOV.txt",					//4
	"C:\\myUndervoltdata\\UserData\\1AC.txt",					//5
	"C:\\myUndervoltdata\\UserData\\2ACS.txt",					//6
	"C:\\myUndervoltdata\\UserData\\3ACS.txt",					//7
	"C:\\myUndervoltdata\\UserData\\4ACS.txt",					//8
	"C:\\myUndervoltdata\\UserData\\TBPM.txt",					//9
	"C:\\myUndervoltdata\\UserData\\TBSPM.txt",					//10
	"C:\\myUndervoltdata\\UserData\\AUTO_VOL.txt",				//11
	"C:\\myUndervoltdata\\UserData\\AUTO_COREFRE.txt",			//12
	"C:\\myUndervoltdata\\UserData\\AUTO_POW.txt",				//13
	"C:\\myUndervoltdata\\UserData"								//14
};

xInterface::xInterface()
{
	system("cls");

	int Option;
	char Confirm;
	xTune Tune;

	HANDLE handle;
	WORD currentConsoleAttribute;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(handle, &csbi))
		currentConsoleAttribute = csbi.wAttributes;

	wcout << endl << L"\t\t\t\t\t" + AppVer + L" - Main Menu" << endl << endl;

	SetConsoleTextAttribute(handle, 10);
	cout << "Current Path: " << xFileOp::opGetDirectory() << endl << endl;
	
	SetConsoleTextAttribute(handle, currentConsoleAttribute);
	cout << "1. Customize CPU Core Voltage Offset" << endl;
	cout << "2. Customize CPU Clock Speed" << endl;
	cout << "3. Customize Turbo Boost Power Max" << endl;
	cout << "4. Customize Turbo Boost Short Power Max" << endl;

	SetConsoleTextAttribute(handle, 14);
	cout << "5. Save & Deploy AutoMode" << endl;

	SetConsoleTextAttribute(handle, currentConsoleAttribute);
	cout << "6. Exit" << endl << endl;
	cout << "Enter an entry: ";

	cin >> Option;

	switch (Option)
	{
	case 1:	//Customize Core Voltage Offset
		int OffsetVal;
	c1:
		system("cls");

		wcout << endl << L"\t\t\t\t  " + AppVer + L" - Customize Core Voltage Offset" << endl << endl;
		cout << "Enter an offset value (-150[-0.150 V] ~ 150[0.150 V]): ";

		cin >> OffsetVal;

		if (OffsetVal < -150 || OffsetVal > 150)
		{
			SetConsoleTextAttribute(handle, 14);
			cout << "Please re-enter the offset value within the supported range (-150~150)";
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			xSysOp::opClean();
			Sleep(3000);
			goto c1;
		}

		SetConsoleTextAttribute(handle, 14);
		cout << "Confirm offset value: " << OffsetVal << " (Y/N) ";
		SetConsoleTextAttribute(handle, currentConsoleAttribute);
		cin >> Confirm;

		if (Confirm == 'y' || Confirm == 'Y')
		{
			cout << endl << "Calibrating and setting offset value..." << endl;
			xSysOp::ExecuteCommand(shExeInfo, 't', 34, OffsetVal);

			SetConsoleTextAttribute(handle, 10);
			cout << endl << "Done!" << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);

			xFileOp::opWriteFile(UserPath[4], OffsetVal);

			Sleep(2000);
			xInterface();
		}
		else if (Confirm == 'n' || Confirm == 'N')
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Canceled..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}
		else
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Invalid option entered..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}
		break;

	case 2:
		int Option, Ratio;
	c2:
		system("cls");

		wcout << endl << L"\t\t\t\t  " + AppVer + L" - Customize CPU Clock Speed" << endl << endl;
		cout << "1. Customize Overall Clock Speed" << endl;
		cout << "2. Customize CPU Clock Multiplier" << endl;
		SetConsoleTextAttribute(handle, 14);
		cout << "3. Main Menu" << endl << endl;
		SetConsoleTextAttribute(handle, currentConsoleAttribute);
		cout << "Enter an entry: ";

		cin >> Option;

		if (Option == 1)	//Customize Overall Clock Speed
		{
			system("cls");

			wcout << endl << L"\t\t  " + AppVer + L" - Customize CPU Clock Speed - Customize Overall Clock Speed" << endl << endl;
			SetConsoleTextAttribute(handle, 14);
			cout << "Warning: This setting will apply to all active cores!" << endl << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			cout << "Input processor clock speed (29[2.9 Ghz] ~ 45[4.5 Ghz]): ";

			cin >> Ratio;

			if (Ratio < 29 || Ratio > 45)
			{
				SetConsoleTextAttribute(handle, 14);
				cout << "Please re-enter the clock speed within the supported range (29~45)";
				SetConsoleTextAttribute(handle, currentConsoleAttribute);
				xSysOp::opClean();
				Sleep(3000);
				goto c2;
			}

			SetConsoleTextAttribute(handle, 14);
			cout << "Confirm clock speed: " << Ratio << " (Y/N) ";
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			cin >> Confirm;

			if (Confirm == 'y' || Confirm == 'Y')
			{
				cout << endl << "Applying CPU clock speed..." << endl;

				for (int ID = 29; ID <= 32; ++ID)
					xSysOp::ExecuteCommand(shExeInfo, 't', ID, Ratio);

				SetConsoleTextAttribute(handle, 10);
				cout << endl << "Done!" << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);

				for (int Path = 5; Path <= 8; ++Path)
					xFileOp::opWriteFile(UserPath[Path], Ratio);

				Sleep(2000);
				xInterface();
			}

			else if (Confirm == 'n' || Confirm == 'N')
			{
				SetConsoleTextAttribute(handle, 12);
				cout << endl << "Canceled..." << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);
				Sleep(1000);
				xInterface();
			}

			else
			{
				xSysOp::opClean();
				SetConsoleTextAttribute(handle, 12);
				cout << "Invalid option entered..." << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);
				Sleep(1000);
				xInterface();
			}
		}

		else if (Option == 2)	//Customize CPU Clock Multiplier
		{
			system("cls");

			int Ratio[4], Num = 1;

			wcout << endl << L"\t\t  " + AppVer + L" - Customize CPU Clock Speed - Customize CPU Clock Multiplier" << endl << endl;

			for (int i = 0; i < 4; ++i)
			{
				cout << Num++ << " Active Core (29[2.9 Ghz] ~ 45[4.5 Ghz]): ";
				cin >> Ratio[i];

				if (Ratio[i] < 29 || Ratio[i] > 45)
				{
					SetConsoleTextAttribute(handle, 14);
					cout << "Please re-enter the clock speed within the supported range (29~45)";
					SetConsoleTextAttribute(handle, currentConsoleAttribute);
					xSysOp::opClean();
					Sleep(3000);
					goto c2;
				}
			}

			SetConsoleTextAttribute(handle, 14);
			cout << endl << "Save and apply? (Yy/Nn)";
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			cin >> Confirm;

			if (Confirm == 'y' || Confirm == 'Y')
			{
				cout << endl << "Applying CPU clock multiplier..." << endl;

				for (int i = 0, ID = 29; i < 4, ID <= 32; ++i, ++ID)
					xSysOp::ExecuteCommand(shExeInfo, 't', ID, Ratio[i]);

				SetConsoleTextAttribute(handle, 10);
				cout << endl << "Done!" << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);

				//Write to UserPath[5, 6, 7, 8]
				for (int i = 0, p = 5; i < 4, p <= 8; ++i, ++p)
					xFileOp::opWriteFile(UserPath[p], Ratio[i]);

				Sleep(2000);
				xInterface();
			}

			else if (Confirm == 'n' || Confirm == 'N')
			{
				SetConsoleTextAttribute(handle, 12);
				cout << endl << "Canceled..." << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);
				Sleep(1000);
				goto c2;
			}

			else
			{
				xSysOp::opClean();
				SetConsoleTextAttribute(handle, 12);
				cout << "Invalid option entered..." << endl;
				SetConsoleTextAttribute(handle, currentConsoleAttribute);
				Sleep(1000);
				goto c2;
			}
		}
		
		else if (Option == 3)
		{
			xSysOp::opClean();
			system("cls");
			xInterface();
		}

		else
		{
			xSysOp::opClean();
			SetConsoleTextAttribute(handle, 10);
			cout << "Please enter a valid number..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			goto c2;
		}

		break;

	case 3:	//Turbo Boost Power Max
		int Power;
	c3:
		system("cls");

		wcout << endl << L"\t\t\t\t  " + AppVer + L" - Customize Turbo Boost Power Max" << endl << endl;
		cout << "Input a power value (1~100W): ";

		cin >> Power;

		if (Power < 1 || Power > 100)
		{
			SetConsoleTextAttribute(handle, 14);
			cout << "Please re-enter the power value within the supported range (1~100W)";
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			xSysOp::opClean();
			Sleep(3000);
			goto c3;
		}

		SetConsoleTextAttribute(handle, 14);
		cout << "Confirm power value: " << Power << "W (Y/N) ";
		SetConsoleTextAttribute(handle, currentConsoleAttribute);
		cin >> Confirm;

		if (Confirm == 'y' || Confirm == 'Y')
		{
			cout << endl << "Setting power value..." << endl;
			xSysOp::ExecuteCommand(shExeInfo, 't', 48, Power);	//Change

			SetConsoleTextAttribute(handle, 10);
			cout << endl << "Done!" << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);

			xFileOp::opWriteFile(UserPath[9], Power);	//Change

			Sleep(2000);
			xInterface();
		}

		else if (Confirm == 'n' || Confirm == 'N')
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Canceled..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		else
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Invalid option entered..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		break;

	case 4:	//Turbo Boost Short Power Max
	c4:
		system("cls");

		wcout << endl << L"\t\t\t\t  " + AppVer + L" - Customize Turbo Boost Short Power Max" << endl << endl;
		cout << "Input a power value (1~100W): ";

		cin >> Power;

		if (Power < 1 || Power > 100)
		{
			SetConsoleTextAttribute(handle, 14);
			cout << "Please re-enter the power value within the supported range (1~100W)";
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			xSysOp::opClean();
			Sleep(3000);
			goto c4;
		}

		SetConsoleTextAttribute(handle, 14);
		cout << "Confirm power value: " << Power << "W (Y/N) ";
		SetConsoleTextAttribute(handle, currentConsoleAttribute);
		cin >> Confirm;

		if (Confirm == 'y' || Confirm == 'Y')
		{
			cout << endl << "Setting power value..." << endl;
			xSysOp::ExecuteCommand(shExeInfo, 't', 47, Power);	//Change

			SetConsoleTextAttribute(handle, 10);
			cout << endl << "Done!" << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);

			xFileOp::opWriteFile(UserPath[10], Power);	//Change

			Sleep(2000);
			xInterface();
		}

		else if (Confirm == 'n' || Confirm == 'N')
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Canceled..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		else
		{
			SetConsoleTextAttribute(handle, 12);
			cout << endl << "Invalid option entered..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		break;

	case 5:	//AutoMode Config
		system("cls");

		wcout << endl << L"\t\t\t\t    " + AppVer + L" - AutoMode Configuration" << endl << endl;
		cout << "You're about to enter AutoMode." << endl;
		cout << "During AutoMode, these settings will be applied everytime after startup & sleep." << endl;

		SetConsoleTextAttribute(handle, 14);
		cout << endl << "1. Voltage" << endl << " ================================";
		cout << endl << "|| Core Voltage Offset Value: " << xFileOp::opReadFile(UserPath[4]) << " ||" << endl;
		cout << " ================================" << endl << endl;

		cout << "2. Core Frequency: " << endl << " ===========================" << endl;
		for (int i = 1, Path = 5; i <= 4, Path <= 8; ++i, ++Path)
			cout << "|| " << i << " Active Core Ratio: " << xFileOp::opReadFile(UserPath[Path]) << " ||" << endl;
		cout << " ===========================" << endl;

		cout << endl << "3. Power: " << endl << " ===================================";
		cout << endl << "|| Turbo Boost Power Max: " << xFileOp::opReadFile(UserPath[9]) << "       ||" << endl;
		cout << "|| Turbo Boost Short Power Max: " << xFileOp::opReadFile(UserPath[10]) << " ||" << endl;
		cout << " ===================================" << endl;

		SetConsoleTextAttribute(handle, 12);
		cout << endl << "Be aware that your CPU might NOT support overclocking!" << endl << endl;
		SetConsoleTextAttribute(handle, currentConsoleAttribute);

		//"if...else" chain sucks :(
		cout << "Enable AutoMode for [Voltage]? (Yy/Nn/0) ";
		cin >> Confirm;
		if (Confirm == 'Y' || Confirm == 'y')
			xFileOp::opWriteFile(UserPath[11], 1);
		else if (Confirm == '0')
			xInterface();
		else
			xFileOp::opWriteFile(UserPath[11], 0);

		cout << "Enable AutoMode for [Core Frequency]? (Yy/Nn/0) ";
		cin >> Confirm;
		if (Confirm == 'Y' || Confirm == 'y')
			xFileOp::opWriteFile(UserPath[12], 1);
		else if (Confirm == '0')
			xInterface();
		else
			xFileOp::opWriteFile(UserPath[12], 0);

		cout << "Enable AutoMode for [Power]? (Yy/Nn/0) ";
		cin >> Confirm;
		if (Confirm == 'Y' || Confirm == 'y')
			xFileOp::opWriteFile(UserPath[13], 1);
		else if (Confirm == '0')
			xInterface();
		else
			xFileOp::opWriteFile(UserPath[13], 0);

		cout << endl << "Save & Enable AutoMode? (Yy/Nn) ";
		cin >> Confirm;

		if (Confirm == 'y' || Confirm == 'Y')
		{
			xFileOp::opWriteFile(UserPath[3], 1);
			SetConsoleTextAttribute(handle, 10);
			cout << endl << "Deploying AutoMode, press any key to cancel..." << endl;
			cout << "Entering background in 5 seconds..." << endl << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			xMisceOp::CountDown(5);
			xSysOp::opSleepDetection();
		}

		else if (Confirm == 'n' || Confirm == 'N')
		{
			xFileOp::opWriteFile(UserPath[3], 0);
			SetConsoleTextAttribute(handle, 12);
			cout << "AutoMode canceled..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		else
		{
			xSysOp::opClean();
			SetConsoleTextAttribute(handle, 12);
			cout << "Invalid option entered..." << endl;
			SetConsoleTextAttribute(handle, currentConsoleAttribute);
			Sleep(1000);
			xInterface();
		}

		break;

	case 6:
		exit(0);

	case 7:
		cout << endl << "You found a easter egg!" << endl;
		cout << "2018/8/14 - This program is made by 1476 lines of code by far!" << endl;
		cout << "Thanks for all your support!" << endl << endl;
		system("pause");
	
	default:
		xSysOp::opClean();
		SetConsoleTextAttribute(handle, 10);
		cout << "Please enter a valid number..." << endl;
		SetConsoleTextAttribute(handle, currentConsoleAttribute);
		Sleep(1000);
		xInterface();
		break;
	}
}

xInterface::~xInterface() {};