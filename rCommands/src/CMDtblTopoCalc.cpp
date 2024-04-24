/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblTopoCalc.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -o, --operation [add|subtract]");
    CMmsgPrint(CMmsgInfo, "     -n, --next      [next fieldname]");
    CMmsgPrint(CMmsgInfo, "     -s, --source    [source fieldname]");
    CMmsgPrint(CMmsgInfo, "     -t, --target    [target fieldname]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt oper = DBMathOperatorAdd;
    char *tableName = (char *) NULL;
    char *srcFieldName  = (char *) NULL;
    char *dstFieldName  = (char *) NULL;
    char *nextFieldName = (char *) NULL;
    DBObjData *data;
    DBObjTable *table;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-a", "--table")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing table name!");
                return (CMfailed);
            }
            tableName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-o", "--operation")) {
            int operCodes[] = {DBMathOperatorAdd, DBMathOperatorSub};
            const char *operations[] = {"add", "subtract", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing operation!");
                return (CMfailed);
            }
            if ((oper = CMoptLookup(operations, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid operation!");
                return (CMfailed);
            }
            oper = operCodes[oper];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-n", "--next")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing next field name!");
                return (CMfailed);
            }
            nextFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-s", "--source")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing source field name!");
                return (CMfailed);
            }
            srcFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-t", "--target")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing target field name!");
                return (CMfailed);
            }
            dstFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
Help:   if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage(argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && ((int) strlen(argv[argPos]) > 1)) {
            CMmsgPrint(CMmsgUsrError, "Unknown option: %s!", argv[argPos]);
            return (CMfailed);
        }
        argPos++;
    }

    if (srcFieldName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing source field!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    if (dstFieldName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing target field!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    if (nextFieldName == (char *) NULL) nextFieldName = RGlibNextStation;
    if (tableName == (char *) NULL) tableName = DBrNItems;

    if (argNum > 3) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    data = new DBObjData();
    if (((argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin)) == DBFault) {
        delete data;
        if (argNum > 1) printf("File error in: %s", argv[1]);
        _CMDprintUsage (argv[0]);
        return (DBFault);
    }

    if ((table = data->Table(tableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid table!");
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }

    switch (oper) {
        default:
        case DBMathOperatorAdd:
            ret = RGlibGenFuncTopoAccum(table, nextFieldName, srcFieldName, dstFieldName);
            break;
        case DBMathOperatorSub:
            ret = RGlibGenFuncTopoSubtract(table, nextFieldName, srcFieldName, dstFieldName);
            break;
    }
    if (ret == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
