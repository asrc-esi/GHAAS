/******************************************************************************

GHAAS Water Balance Model Library V1.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MFDomain.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <string.h>
#include <cm.h>
#include <MF.h>

static int _MFDefLevel = 0;

void _MFDefEntering (const char *text,const char *path) {
	char formatStr [MFNameLength];

	snprintf (formatStr, sizeof(formatStr), "%c%ds %cs (%s)",'%',_MFDefLevel * 3 + (int) strlen ("Entering"),'%',CMfileName(path));
	CMmsgPrint (CMmsgInfo,formatStr,"Entering:",text);
	_MFDefLevel++;
}

void _MFDefLeaving (const char *text, const char *path) {
	char formatStr [MFNameLength];

	_MFDefLevel--;
	snprintf (formatStr, sizeof(formatStr), "%c%ds %cs (%s)",'%',_MFDefLevel * 3 + (int) strlen ("Entering"),'%',CMfileName (path));
	CMmsgPrint (CMmsgInfo,formatStr,"Leaving:",text);
}
