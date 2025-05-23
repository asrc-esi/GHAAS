/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDrgis2sql.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <rgis file> <sqlfile>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --rgistable  [rgis table]");
    CMmsgPrint(CMmsgInfo, "     -c, --case       [sensitive|lower|upper");
    CMmsgPrint(CMmsgInfo, "     -s, --schema     [sql schema]");
    CMmsgPrint(CMmsgInfo, "     -q, --sqltable   [sql table]");
    CMmsgPrint(CMmsgInfo, "     -d, --dialect    [postgres|sqlite]");
    CMmsgPrint(CMmsgInfo, "     -m, --mode       [copy|append|replace|blank]");
    CMmsgPrint(CMmsgInfo, "     -r, --recordname [on|off]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    FILE *outFile;
    DBInt argPos, argNum = argc, ret;
    RGlibTableAction  mode = RGlibTableCopy;
    bool         recordName = true;
    RGlibSQLdialect dialect = RGlibSQLpostgres;
    RGlibSQLCase    sqlCase = RGlibSQLCaseSensitive;
    char *rgisTableName     = (char *) "DBItems";
    char *dbSchemaName      = (char *) NULL;
    char *sqlTableName   = (char *) NULL;
    DBObjData *data = (DBObjData *) NULL;
    DBObjTable *table;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-a", "--rgistable")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing network!");
                return (CMfailed);
            }
            sqlTableName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-c", "--case")) {
            DBInt code;
            RGlibSQLCase codes[] = {RGlibSQLCaseSensitive,
                           RGlibSQLCaseLower,
                           RGlibSQLCaseUpper};
            const char *options[] = {"sensitive", "lower", "upper", (char *) NULL};
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "option!");
                return (CMfailed);
            }
            if ((code = CMoptLookup(options, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid option!");
                return (CMfailed);
            }
            sqlCase = codes[code];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-q", "--sqltable")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing network!");
                return (CMfailed);
            }
            sqlTableName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-s", "--schema")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing network!");
                return (CMfailed);
            }
            dbSchemaName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest(argv[argPos], "-m", "--mode")) {
            const char *options[] = {"copy", "append", "replace", "blank", (char *) NULL};
            DBInt code;
            RGlibTableAction codes[] = {RGlibTableCopy, RGlibTableAppend, RGlibTableReplace, RGlibTableBlank};
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            if ((code = CMoptLookup(options, argv[argPos], false)) == CMfailed) {
                CMmsgPrint(CMmsgWarning, "Ignoring illformed step option [%s]!", argv[argPos]);
            }
                else mode = codes[code];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest(argv[argPos], "-r", "--recordname")) {
            const char *options[] = {"on", "off", (char *) NULL};
            DBInt code;
            bool codes[] = {true,false};
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            if ((code = CMoptLookup(options, argv[argPos], false)) == CMfailed) {
                CMmsgPrint(CMmsgWarning, "Ignoring illformed step option [%s]!", argv[argPos]);
            }
            else recordName = codes[code];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest(argv[argPos], "-d", "--dialect")) {
            const char *options[] = {"postgres", "sqlite", (char *) NULL};
            DBInt code;
            RGlibSQLdialect codes[] = {RGlibSQLpostgres, RGlibSQLite};
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            if ((code = CMoptLookup(options, argv[argPos], false)) == CMfailed) {
                CMmsgPrint(CMmsgWarning, "Ignoring illformed step option [%s]!", argv[argPos]);
            }
            else dialect = codes[code];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-h", "--help")) {
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
    if ((ret == DBFault) || ((table = data->Table (rgisTableName)) == (DBObjTable *) NULL)) {
        CMmsgPrint(CMmsgUsrError, "Wrong rgis table: %s!",rgisTableName);
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }
    if ((outFile = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? fopen (argv[2],"w") : stdout) == (FILE *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid output!");
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }

    ret = RGlibTableToSQL (table, dbSchemaName, sqlTableName, mode, recordName, dialect, sqlCase, outFile);

    delete data;
    if (outFile != stdout) {
        fclose (outFile);
        if (ret == CMfailed) unlink (argv[2]);
    }
    return (ret);
}
