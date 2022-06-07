/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2022, UNH - ASRC/CUNY

CMDtblConvCoord.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [coord field name]");
    CMmsgPrint(CMmsgInfo, "     -r, --rename    [output field]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt recID, i, deg, min, sec, strLen;
    DBFloat coord;
    char crdStr[DBStringLength];
    char *minStr, *secStr;
    char *tableName = (char *) NULL;
    char *srcFieldName = (char *) NULL;
    char *dstFieldName = (char *) NULL;
    DBObjData *data;
    DBObjTable *table;
    DBObjTableField *srcField;
    DBObjTableField *dstField;
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
            srcFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-r", "--rename")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing second field name!");
                return (CMfailed);
            }
            dstFieldName = argv[argPos];
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
    if (srcFieldName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Coordinate field is not set!");
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

    if ((srcField = table->Field(srcFieldName)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid coordinate field [%s]!", srcFieldName);
        delete data;
        return (CMfailed);
    }


    if (dstFieldName == (char *) NULL) dstFieldName = (char *) "DecimalCoord";
    {
        if ((dstField = table->Field(dstFieldName)) == (DBObjTableField *) NULL) {
            dstField = new DBObjTableField(dstFieldName, DBVariableFloat, "%10.3f", sizeof(DBFloat4), false);
            table->AddField(dstField);
        }
        else {
            if (dstField->Type() != DBVariableFloat) {
                CMmsgPrint(CMmsgUsrError, "Invalid coordinate field type!");
                delete data;
                return (CMfailed);
            }
        }
    }

    for (recID = 0; recID < table->ItemNum(); ++recID) {
        record = table->Item(recID);
        strncpy(crdStr, srcField->String(record), sizeof(crdStr) - 1);
        if ((strLen = strlen(crdStr)) > 0) {
            minStr = secStr = (char *) NULL;
            for (i = 0; i < strLen; ++i)
                if (crdStr[i] == ':') minStr = crdStr + i + 1;
                else if (crdStr[i] == '\'') secStr = crdStr + i + 1;
                else if (crdStr[i] == '\"') crdStr[i] = '\0';
            if (sscanf(crdStr, "%d", &deg) != 1) continue;
            coord = (float) deg;
            if ((minStr != (char *) NULL) && (sscanf(minStr, "%d", &min) == 1)) {
                coord = coord + (deg > 0 ? 1.0 : -1.0) * (DBFloat) min / 60.0;
                if ((secStr != (char *) NULL) && (sscanf(secStr, "%d", &sec) == 1))
                    coord = coord + (deg > 0 ? 1.0 : -1.0) * (DBFloat) sec / 3600.0;
            }
            dstField->Float(record, coord);
        }
    }

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
