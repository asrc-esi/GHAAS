/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblDeleteField.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret, i, fieldNum = 0;
    DBObjData *data;
    char *tableName = (char *) NULL, **fieldNames;
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
            fieldNames = fieldNum > 0 ? (char **) realloc(fieldNames, (fieldNum + 1) * sizeof(char *)) :
                         (char **) calloc(1, sizeof(char *));
            if (fieldNames == (char **) NULL) {
                CMmsgPrint(CMmsgSysError, "Memory allocation error in: %s %d", __FILE__, __LINE__);
                return (CMfailed);
            }
            fieldNames[fieldNum++] = argv[argPos];
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
    if (fieldNum < 1) {
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
        delete data;
        return (CMfailed);
    }

    for (i = 0; i < fieldNum; ++i) {
        if ((field = table->Field(fieldNames[i])) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid field: %s!", fieldNames[i]);
            continue;
        }
        if (DBTableFieldIsOptional(field) != true) {
            CMmsgPrint(CMmsgUsrError, "Required field!");
            continue;
        }
        table->DeleteField(field);
    }

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    free(fieldNames);
    delete data;
    return (ret);
}
