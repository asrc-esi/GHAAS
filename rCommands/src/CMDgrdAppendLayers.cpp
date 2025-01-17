/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDgrdAppendLayers.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <append grid0> ..... <append gridN>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -o, --output    [ouptput grid]");
    CMmsgPrint(CMmsgInfo, "     -t, --title     [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject   [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain    [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version   [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret, data, dataNum = 0;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    char *output = (char *) NULL, **dataList = (char **) NULL;
    DBObjData *grdData, *appData;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-o", "--output")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing output grid!");
                return (CMfailed);
            }
            output = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-t", "--title")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing title!");
                return (CMfailed);
            }
            title = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-u", "--subject")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing subject!");
                return (CMfailed);
            }
            subject = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-d", "--domain")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing domain!");
                return (CMfailed);
            }
            domain = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-v", "--version")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing version!");
                return (CMfailed);
            }
            version = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
Help:   if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage (argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && (strlen(argv[argPos]) > 1)) {
            CMmsgPrint(CMmsgUsrError, "Unknown option: %s!", argv[argPos]);
            return (CMfailed);
        }
        argPos++;
    }

    dataNum = argNum - 1;
    if ((dataList = (char **) realloc(dataList, dataNum * sizeof(char *))) == (char **) NULL) {
        CMmsgPrint(CMmsgSysError, "Memory allocation error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    for (data = 0; data < dataNum; ++data) {
        dataList[data] = argv[data + 1];
    }
    grdData = new DBObjData();
    ret = (strcmp(dataList[0], "-") != 0) ? grdData->Read(dataList[0]) : grdData->Read(stdin);
    if ((ret == DBFault) || ((grdData->Type() & DBTypeGrid) != DBTypeGrid)) {
        delete grdData;
        return (CMfailed);
    }

    if (title != (char *) NULL) grdData->Name(title);
    if (subject != (char *) NULL) grdData->Document(DBDocSubject, subject);
    if (domain != (char *) NULL) grdData->Document(DBDocGeoDomain, domain);
    if (version != (char *) NULL) grdData->Document(DBDocVersion, version);

    for (data = 1; data < dataNum; ++data) {
        appData = new DBObjData();
        if (appData->Read(dataList[data]) == DBFault) {
            delete grdData;
            delete appData;
            return (CMfailed);
        }
        if (DBGridAppend(grdData, appData) == DBFault) {
            delete grdData;
            delete appData;
            return (CMfailed);
        }
        delete appData;
    }

    ret = (output != (char *) NULL) && (strcmp(argv[2], "-") != 0) ? grdData->Write(output) : grdData->Write(stdout);

    delete grdData;
    return (ret);
}
