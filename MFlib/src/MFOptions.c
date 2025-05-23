/******************************************************************************

GHAAS Water Balance Model Library V1.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MFOptions.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <cm.h>
#include <MF.h>

typedef struct MFOption_s {
	char *Name;
	char *Content;
	bool InUse;
} MFOption_t, *MFOption_p;

static MFOption_p _MFOptions = (MFOption_p) NULL;
static int _MFOptionNum = 0;

const char *MFcalcOptions   [] = { MFhelpStr, MFinputStr, MFcalculateStr, MFnoneStr, (char *) NULL };
const char *MFsourceOptions [] = { MFhelpStr, MFinputStr, MFcalculateStr, (char *) NULL };
const char *MFlookupOptions [] = { MFhelpStr, MFinputStr, MFlookupStr,    (char *) NULL };
const char *MFswitchOptions [] = { MFhelpStr, MFnoneStr,  MFcalculateStr, (char *) NULL };

bool _MFOptionTestInUse () {
	int opt;
	bool ret = true;

	for (opt = 0;opt < _MFOptionNum;++opt)
		if (_MFOptions [opt].InUse == false) {
			CMmsgPrint (CMmsgWarning,"Warning: Unused option [%s]!",_MFOptions [opt].Name);
			ret = false;
		}
	return (ret);
}

static int _MFOptionNew (const char *name,const char *content) {
	int opt;

	for (opt = 0;opt < _MFOptionNum;++opt)
		if (strcmp (_MFOptions [opt].Name,name) == 0) {
			CMmsgPrint (CMmsgWarning,"Waring: Ignoring redefinition of option [%s]!",name);
			return (CMfailed);
		}
	if ((_MFOptions = (MFOption_p) realloc (_MFOptions,(_MFOptionNum + 1) * sizeof (MFOption_t))) == (MFOption_p) NULL) {
		CMmsgPrint (CMmsgSysError,"Memory allocation error in: %s:%d",__FILE__,__LINE__);
		return (CMfailed);
	}
	_MFOptions [_MFOptionNum].Name = _MFOptions [_MFOptionNum].Content = (char *) NULL;
	if ((_MFOptions [_MFOptionNum].Name    = malloc (strlen (name)    + 1)) == (char *) NULL) {
		CMmsgPrint (CMmsgSysError,"Memory allocation error in: %s:%d",__FILE__,__LINE__);
		return (CMfailed);
	}
	strcpy (_MFOptions [_MFOptionNum].Name,    name);
	if (content != (char *) NULL) {
		if ((_MFOptions [_MFOptionNum].Content = malloc (strlen (content) + 1)) == (char *) NULL) {
			CMmsgPrint (CMmsgSysError,"Memory allocation error in: %s:%d",__FILE__,__LINE__);
			return (CMfailed);
		}
		strcpy (_MFOptions [_MFOptionNum].Content, content);
	}
	_MFOptions [_MFOptionNum].InUse   = false;
	_MFOptionNum++;
	return (CMsucceeded);
}

const char *MFOptionGet (const char *name) {
	int opt;
	for (opt = 0;opt < _MFOptionNum;++opt) if (strcmp (_MFOptions [opt].Name,name) == 0) break;
	if (opt == _MFOptionNum)	_MFOptionNew (name,(char *) NULL);
	_MFOptions [opt].InUse = true;
	return (_MFOptions [opt].Content);
}

void MFOptionPrintList () {
	int opt;
	CMmsgPrint (CMmsgInfo,"Options:");
	for (opt = 0;opt < _MFOptionNum;++opt) if (_MFOptions [opt].InUse) CMmsgPrint (CMmsgInfo," %s",_MFOptions [opt].Name);
}

int MFOptionParse (int argc, char *argv []) {
	int argNum = argc, argPos, i;

	for (argPos = 1;argPos < argNum;) {
		if (CMargTest (argv [argPos],"-p","--option")) {
			if ((argNum = CMargShiftLeft (argPos,argv,argNum)) < 1) {
				CMmsgPrint (CMmsgUsrError,"Missing option argument!");
				return (CMfailed);
			}
			for (i = 0;i < (int) strlen (argv[argPos]);++i) if (argv [argPos][i] == '=') break;
			if (i == (int) strlen (argv [argPos])) {
				CMmsgPrint (CMmsgAppError,"Illformed option [%s]!",argv [argPos]);
				return (CMfailed);
			}
			argv [argPos][i] = '\0';
			_MFOptionNew (argv [argPos],argv [argPos] + i + 1);
			if ((argNum = CMargShiftLeft(argPos,argv,argNum)) <= argPos) break;
			continue;
		}
		argPos++;
	}
	return (argNum);
}

void MFOptionMessage (const char *optName, const char *optStr, const char *options []) {
	int opt;
	if ((optStr == (char *) NULL) || (strcmp (optStr,MFhelpStr) == 0)) CMoptPrintList (CMmsgInfo,optName,options);
	else CMmsgPrint (CMmsgUsrError, "Invalid [%s] option: %s\nValid options: ", optName, optStr);
}
