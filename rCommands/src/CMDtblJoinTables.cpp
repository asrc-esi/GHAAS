/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblJoinTables.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --dataset   <join dataset>");
    CMmsgPrint(CMmsgInfo, "     -e, --relate    [relate table]");
    CMmsgPrint(CMmsgInfo, "     -o, --join      [join table]");
    CMmsgPrint(CMmsgInfo, "     -r, --rfield    [relate field]");
    CMmsgPrint(CMmsgInfo, "     -j, --jfield    [join field]");
    CMmsgPrint(CMmsgInfo, "     -t, --title     [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject   [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain    [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version   [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    DBObjData *data, *joinData;
    char *joinDataName = (char *) NULL;
    char *relateTableName = (char *) NULL, *joinTableName = (char *) NULL;
    char *relateFieldName = (char *) NULL, *joinFieldName = (char *) NULL;
    DBObjTable *relateTable, *joinTable;
    DBObjTableField *relateField, *joinField;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-a", "--dataset")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing join dataset!");
                return (CMfailed);
            }
            joinDataName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-e", "--relate")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing relate table name!");
                return (CMfailed);
            }
            relateTableName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-o", "--join")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing join table name!");
                return (CMfailed);
            }
            joinTableName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-r", "--rfield")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing relate field name!");
                return (CMfailed);
            }
            relateFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-j", "--jfield")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing join field name!");
                return (CMfailed);
            }
            joinFieldName = argv[argPos];
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

    if (title != (char *) NULL) data->Name(title);
    if (subject != (char *) NULL) data->Document(DBDocSubject, subject);
    if (domain != (char *) NULL) data->Document(DBDocGeoDomain, domain);
    if (version != (char *) NULL) data->Document(DBDocVersion, version);

    if (joinDataName == (char *) NULL) joinData = data;
    else {
        joinData = new DBObjData();
        if (joinData->Read(joinDataName) == DBFault) {
            CMmsgPrint(CMmsgUsrError, "Invalid join dataset!");
            delete data;
            delete joinData;
            return (CMfailed);
        }
    }

    if (relateTableName == (char *) NULL) relateTableName = DBrNItems;
    if (joinTableName == (char *) NULL) joinTableName = DBrNItems;
    if ((data == joinData) && (strcmp(relateTableName, joinTableName) == 0)) {
        CMmsgPrint(CMmsgUsrError, "Join tables are identical");
        if (joinData != data)
            delete joinData,
                    delete data;
        return (CMfailed);
    }

    if ((relateTable = data->Table(relateTableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid relate table: %s", relateTableName);
        if (joinData != data) delete joinData;
        delete data;
        return (CMfailed);
    }

    if ((joinTable = joinData->Table(joinTableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid join table: %s!", joinTableName);
        if (joinData != data) delete joinData;
        delete data;
        return (CMfailed);
    }

    if (relateFieldName != (char *) NULL) {
        if ((relateField = relateTable->Field(relateFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid relate field: %s!", relateFieldName);
            if (joinData != data) delete joinData;
            delete data;
            return (CMfailed);
        }
    }
    else relateField = (DBObjTableField *) NULL;

    if (joinFieldName != (char *) NULL) {
        if ((joinField = joinTable->Field(joinFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid join field: %s!", joinFieldName);
            if (joinData != data) delete joinData;
            delete data;
            return (CMfailed);
        }
    }
    else joinField = (DBObjTableField *) NULL;

    if ((ret = RGLibTableJoin(relateTable, relateField, joinTable, joinField)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
