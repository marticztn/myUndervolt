#include <iostream>

#include "xInterface.h"
#include "xOperation.h"

using namespace std;

int main(int argc, char** argv)
{
	xInitOp InitOp;
	xMisceOp Mis;

	InitOp.opDetectRelatedProcess();
	InitOp.opInitializeWindow();
	InitOp.opValidateFiles();
	InitOp.opEstablishService();

	xInterface();

	return 0;
}