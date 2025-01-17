/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblAddField.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -y, --type      [field type]");
    CMmsgPrint(CMmsgInfo, "     -l, --length    [field length]");
    CMmsgPrint(CMmsgInfo, "     -w, --width     [display width]");
    CMmsgPrint(CMmsgInfo, "     -e, --decimals  [display decimals]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt type = DBFault, length = DBFault, dWidth = DBFault, dDecimals = DBFault;
    DBObjData *data;
    char *tableName = (char *) NULL, *fieldName = (char *) NULL;
    DBObjTable *table;
    DBObjTableField *field;

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
        if (CMargTest (argv[argPos], "-f", "--field")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing field name!");
                return (CMfailed);
            }
            fieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-y", "--type")) {
            DBInt typeCodes[] = {DBTableFieldString, DBTableFieldInt, DBTableFieldFloat, DBTableFieldDate};
            const char *types[] = {"string", "int", "float", "date", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing field type!");
                return (CMfailed);
            }
            if ((type = CMoptLookup(types, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid field type!");
                return (CMfailed);
            }
            type = typeCodes[type];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-l", "--length")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing field length!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%d", &length) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid field length!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-w", "--width")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing format width!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%d", &dWidth) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid format width!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-e", "--decimals")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing decimals!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%d", &dDecimals) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid number of decimals!");
                return (CMfailed);
            }
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
    if (tableName == (char *) NULL) tableName = DBrNItems;
    if (fieldName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing field name!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    data = new DBObjData();
    if (((argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin)) == DBFault) {
        delete data;
        return (CMfailed);
    }

    if ((table = data->Table(tableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid table: %s!", tableName);
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }

    if (type == DBFault) { type = DBTableFieldString; }
    if (length == DBFault)
        switch (type) {
            default:
            case DBTableFieldString:
                length = 16;
                break;
            case DBTableFieldInt:
                length = 2;
                break;
            case DBTableFieldFloat:
                length = 4;
                break;
            case DBTableFieldDate:
                length = sizeof(DBDate);
                break;
        }
    else
        switch (type) {
            default:
            case DBTableFieldString:
                break;
            case DBTableFieldInt:
                if ((length != 1) && (length != 2) && (length != 4)) {
                    CMmsgPrint(CMmsgUsrError, "Invalid field length");
                    delete data;
                    return (CMfailed);
                }
                break;
            case DBTableFieldFloat:
                if ((length != 4) && (length != 8)) {
                    CMmsgPrint(CMmsgUsrError, "Invalid field length");
                    delete data;
                    return (CMfailed);
                }
                break;
            case DBTableFieldDate:
                if (length != sizeof(DBDate)) {
                    CMmsgPrint(CMmsgUsrError, "Invalid field length");
                    delete data;
                    return (CMfailed);
                }
                break;
        }
    if (dWidth == DBFault)
        switch (type) {
            default:
            case DBTableFieldString:
                dWidth = length;
                break;
            case DBTableFieldInt:
                dWidth = 8;
                break;
            case DBTableFieldFloat:
                dWidth = 10;
                break;
            case DBTableFieldDate:
                dWidth = 10;
                break;
        }
    if (dDecimals != DBFault) {
        if (type != DBTableFieldFloat) {
            CMmsgPrint(CMmsgUsrError, "Invalid field type for decimals!");
            delete data;
            return (CMfailed);
        }
        dDecimals = 2;
    }
    field = new DBObjTableField(fieldName, length);
    field->Type(type);
    field->FormatWidth(dWidth);
    field->Length(length);
    if (type == DBTableFieldFloat) field->FormatDecimals(dDecimals);
    table->AddField(field);

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
