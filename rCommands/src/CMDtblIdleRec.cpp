/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblIdleRec.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -m, --mode      [set|clear]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret, idleMode;
    char *tableName = (char *) NULL;
    DBInt recID;
    DBObjData *data;
    DBObjTable *table;
    DBObjRecord *record;

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
        if (CMargTest (argv[argPos], "-m", "--mode")) {
            int modeCodes[] = {true, false};
            const char *modes[] = {"set", "clear"};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing selection mode!");
                return (CMfailed);
            }
            if ((idleMode = CMoptLookup(modes, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid selection mode!");
                return (CMfailed);
            }
            idleMode = modeCodes[idleMode];
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
    if (((argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin)) == DBFault) {
        delete data;
        return (CMfailed);
    }

    if (tableName == (char *) NULL) tableName = DBrNItems;

    if ((table = data->Table(tableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid table!");
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }

    if (idleMode)
        for (recID = 0; recID < table->ItemNum(); ++recID) {
            record = table->Item(recID);
            if ((record->Flags() & DBObjectFlagSelected) == DBObjectFlagSelected)
                record->Flags(DBObjectFlagIdle, DBSet);
            else
                record->Flags(DBObjectFlagIdle, DBClear);
        }
    else
        for (recID = 0; recID < table->ItemNum(); ++recID) {
            record = table->Item(recID);
            record->Flags(DBObjectFlagIdle, DBClear);
        }

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
