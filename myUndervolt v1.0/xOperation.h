#pragma once
#ifndef _X_OPERATION_H
#define _X_OPERATION_H

#include <Windows.h>
#include <string>

class xFileOp
{
public:
	xFileOp();
	~xFileOp();

	static int opReadFile(const char*);
	static void opWriteFile(const char*, int);
	void opWriteDirectory(std::string);
	static std::string opGetDirectory(void);
	std::string opGetClipboardData(void);

private:
	//TODO...
};

class xInitOp : public xFileOp
{
public:
	xInitOp();
	~xInitOp();

	void opDetectRelatedProcess(void);
	void opInitializeWindow(void);
	void opValidateFiles(void);
	static void opEstablishService(void);

private:
	//TODO...
};

class xSysOp
{
public:
	xSysOp();
	~xSysOp();

	static void opClean(void);
	static HWND opFindWindow(LPCTSTR);
	static void opSleepDetection(void);
	static void opWait(HANDLE);
	static std::string ExecuteCommand(SHELLEXECUTEINFO, char, unsigned long, int);
	static LRESULT _stdcall opSleepDetectionProc(HWND, UINT, WPARAM, LPARAM);
	
private:
	//TODO...
};

class xMisceOp
{
public:
	xMisceOp();
	~xMisceOp();

	static void CountDown(int Num);
};

#endif // !_FILE_OP