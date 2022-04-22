#pragma once
#ifndef _X_TUNE_H
#define _X_TUNE_H

#include <Windows.h>
#include <string>

class xTune
{
public:
	xTune();
	~xTune();

	void CoreVoltageOffset(int);
	void ProcessorCoreRatio(int);

private:
	//TODO...
};

#endif // !_TUNE_H