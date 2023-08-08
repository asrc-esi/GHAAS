/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

CMDgrdOperation.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <continuous grid>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -o, --operation [+|-|*|/|^]");
    CMmsgPrint(CMmsgInfo, "     -g, --grid      <grid name>");
    CMmsgPrint(CMmsgInfo, "     -c, --constant  [constant value]");
    CMmsgPrint(CMmsgInfo, "     -m, --merge     [true|false]");
    CMmsgPrint(CMmsgInfo, "     -t, --title     [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject   [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain    [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version   [version]");
    CMmsgPrint(CMmsgInfo, "     -s, --shadeset   [standard|grey|blue|blue-to-red|elevation]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    int operation = DBFault, constSet = false, merge = DBFault;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    DBInt shadeSet = DBFault;
    char *gridName = (char *) NULL;
    float constant;
    DBObjData *data, *gridData;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-o", "--operation")) {
            int operCodes[] = {DBMathOperatorAdd, DBMathOperatorSub,
                               DBMathOperatorMul, DBMathOperatorDiv,
                               DBMathOperatorExp};
            const char *operations[] = {"+", "-", "*", "/", "^", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing operation!");
                return (CMfailed);
            }
            if ((operation = CMoptLookup(operations, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid operation!");
                return (CMfailed);
            }
            operation = operCodes[operation];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-g", "--grid")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing grid!");
                return (CMfailed);
            }
            gridName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-c", "--constant")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing constant!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%f", &constant) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid constant!");
                return (CMfailed);
            }
            constSet = true;
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-m", "--merge")) {
            int mergeCodes[] = {true, false};
            const char *mergeStrs[] = {"true", "false"};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing operation!");
                return (CMfailed);
            }
            if ((merge = CMoptLookup(mergeStrs, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid merge mode!");
                return (CMfailed);
            }
            merge = mergeCodes[merge];
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
        if (CMargTest (argv[argPos], "-s", "--shadeset")) {
            int shadeCodes[] = {DBDataFlagDispModeContStandard,
                                DBDataFlagDispModeContGreyScale,
                                DBDataFlagDispModeContBlueScale,
                                DBDataFlagDispModeContBlueRed,
                                DBDataFlagDispModeContElevation};
            const char *shadeSets[] = {"standard", "grey", "blue", "blue-to-red",
                                       "elevation", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing shadeset!");
                return (CMfailed);
            }
            if ((shadeSet = CMoptLookup(shadeSets, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid shadeset!");
                return (CMfailed);
            }
            shadeSet = shadeCodes[shadeSet];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
Help:   if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage(argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && (strlen(argv[argPos]) > 1)) {
            CMmsgPrint(CMmsgUsrError, "Unknown option: %s!", argv[argPos]);
            return (CMfailed);
        }
        argPos++;
    }

    if (argNum > 3) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    data = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin);
    if ((ret == DBFault) || (data->Type() != DBTypeGridContinuous)) {
        delete data;
        return (CMfailed);
    }

    if (constSet) DBGridOperation(data, constant, operation);
    if (gridName != (char *) NULL) {
        gridData = new DBObjData();
        if ((gridData->Read(gridName) != DBSuccess) || (gridData->Type() != DBTypeGridContinuous)) {
            delete data;
            delete gridData;
            return (CMfailed);
        }
        DBGridOperation(data, gridData, operation, merge);
        delete gridData;
    }

    if (title != (char *) NULL) data->Name(title);
    if (subject != (char *) NULL) data->Document(DBDocSubject, subject);
    if (domain != (char *) NULL) data->Document(DBDocGeoDomain, domain);
    if (version != (char *) NULL) data->Document(DBDocVersion, version);
    if (shadeSet != DBFault) {
        data->Flags(DBDataFlagDispModeContShadeSets, DBClear);
        data->Flags(shadeSet, DBSet);
    }

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
