/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblDecimalDate.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -o, --outfield  [output field]");
    CMmsgPrint(CMmsgInfo, "     -m, --mode      [year|month]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt recID, mode = DBTimeStepYear;
    char *tableName = (char *) NULL;
    char *srcName = (char *) NULL;
    char *dstName = (char *) NULL;
    DBObjData *data;
    DBObjTable *table;
    DBObjTableField *srcField;
    DBObjTableField *dstField = (DBObjTableField *) NULL;
    DBDate date;
    DBObjRecord *record;

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
        if (CMargTest (argv[argPos], "-f", "--field")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing field name!");
                return (CMfailed);
            }
            srcName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-o", "--outfield")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing output field name!");
                return (CMfailed);
            }
            dstName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-m", "--mode")) {
            int optCodes[] = {DBTimeStepYear, DBTimeStepMonth};
            const char *optStrs[] = {"year", "month", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing mode!");
                return (CMfailed);
            }
            if ((mode = CMoptLookup(optStrs, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid decimal date method!");
                return (CMfailed);
            }
            mode = optCodes[mode];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage(argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && ((int) strlen(argv[argPos]) > 1)) {
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
        delete data;
        return (CMfailed);
    }

    if (srcName == (char *) NULL) srcName = (char *) "Date";
    if ((srcField = table->Field(srcName)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing date field!");
        delete data;
        return (CMfailed);
    }
    if ((srcField->Type() != DBTableFieldString) &&
        (srcField->Type() != DBTableFieldDate)) {
        CMmsgPrint(CMmsgUsrError, "Invalid date field!");
        delete data;
        return (CMfailed);
    }

    if (dstName == (char *) NULL)
        switch (mode) {
            case DBTimeStepYear:
                dstName = (char *) "Decimal Year";
                break;
            case DBTimeStepMonth:
                dstName = (char *) "Decimal Month";
                break;
        }

    if ((dstField = table->Field(dstName)) == (DBObjTableField *) NULL) {
        dstField = new DBObjTableField(dstName, DBTableFieldFloat, (char *) "%8.2f", sizeof(DBFloat4), false);
        table->AddField(dstField);
    }

    for (recID = 0; recID < table->ItemNum(); ++recID) {
        record = table->Item(recID);
        if (srcField->Type() == DBTableFieldString)
            date.Set(srcField->String(record));
        else date = srcField->Date(record);
        switch (mode) {
            case DBTimeStepYear:
                dstField->Float(record, date.DecimalYear());
                break;
            case DBTimeStepMonth:
                dstField->Float(record, date.DecimalMonth());
                break;
        }
    }
    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
