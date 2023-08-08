/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

CMDtblMergeDate.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -Y, --year      [year field]");
    CMmsgPrint(CMmsgInfo, "     -M, --month     [month field]");
    CMmsgPrint(CMmsgInfo, "     -D, --day       [day field]");
    CMmsgPrint(CMmsgInfo, "     -H, --hour      [hour field]");
    CMmsgPrint(CMmsgInfo, "     -I, --minute    [minute field]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt recID, year, month, day, hour, min;
    char *tableName = (char *) NULL;
    char *fieldName = (char *) NULL;
    char *yearFieldName = (char *) NULL;
    char *monthFieldName = (char *) NULL;
    char *dayFieldName = (char *) NULL;
    char *hourFieldName = (char *) NULL;
    char *minFieldName = (char *) NULL;
    DBObjData *data;
    DBObjTable *table;
    DBObjTableField *dstField;
    DBObjTableField *yearField = (DBObjTableField *) NULL;
    DBObjTableField *monthField = (DBObjTableField *) NULL;
    DBObjTableField *dayField = (DBObjTableField *) NULL;
    DBObjTableField *hourField = (DBObjTableField *) NULL;
    DBObjTableField *minField = (DBObjTableField *) NULL;
    DBDate date;
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
        if (CMargTest (argv[argPos], "-f", "--field")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing field name!");
                return (CMfailed);
            }
            fieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-Y", "--year")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing year field name!");
                return (CMfailed);
            }
            yearFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-M", "--month")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing month field name!");
                return (CMfailed);
            }
            monthFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-D", "--day")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing day field name!");
                return (CMfailed);
            }
            dayFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-H", "--hour")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing hour field name!");
                return (CMfailed);
            }
            hourFieldName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-I", "--minute")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing minute field name!");
                return (CMfailed);
            }
            minFieldName = argv[argPos];
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

    if (yearFieldName != (char *) NULL) {
        if ((yearField = table->Field(yearFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid year field [%s]!", yearFieldName);
            _CMDprintUsage (argv[0]);
            delete data;
            return (CMfailed);
        }
    }

    if (monthFieldName != (char *) NULL) {
        if ((monthField = table->Field(monthFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid month field [%s]!", monthFieldName);
            _CMDprintUsage (argv[0]);
            delete data;
            return (CMfailed);
        }
    }

    if (dayFieldName != (char *) NULL) {
        if (monthField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Month field is not set!");
            _CMDprintUsage (argv[0]);
            delete data;
            return (CMfailed);
        }
        if ((dayField = table->Field(dayFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid day field [%s]", dayFieldName);
            delete data;
            return (CMfailed);
        }
    }

    if (hourFieldName != (char *) NULL) {
        if (dayField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Day field is not set!");
            delete data;
            return (CMfailed);
        }
        if ((hourField = table->Field(hourFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid hour field [%s]", hourFieldName);
            delete data;
            return (CMfailed);
        }
    }

    if (minFieldName != (char *) NULL) {
        if (hourField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Hour field is not set!");
            _CMDprintUsage (argv[0]);
            delete data;
            return (CMfailed);
        }
        if ((minField = table->Field(minFieldName)) == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Invalid minute field [%s]", minFieldName);
            _CMDprintUsage (argv[0]);
            delete data;
            return (CMfailed);
        }
    }

    if (fieldName == (char *) NULL) fieldName = (char *) "Date";
    if ((dstField = table->Field(fieldName)) == (DBObjTableField *) NULL) {
        dstField = new DBObjTableField(fieldName, DBTableFieldDate, "%s", sizeof(DBDate), false);
        table->AddField(dstField);
    }

    for (recID = 0; recID < table->ItemNum(); ++recID) {
        record = table->Item(recID);
        if (yearField != (DBObjTableField *) NULL) {
            year = yearField->Int(record);
            if (year == yearField->IntNoData()) year = DBDefaultMissingIntVal;
        }
        else year = DBDefaultMissingIntVal;
        if (monthField != (DBObjTableField *) NULL) {
            month = monthField->Int(record);
            month = month == monthField->IntNoData() ? DBDefaultMissingIntVal : month - 1;
        }
        else month = DBDefaultMissingIntVal;
        if (dayField != (DBObjTableField *) NULL) {
            day = dayField->Int(record);
            if (day == dayField->IntNoData()) day = DBDefaultMissingIntVal;
        }
        else day = DBDefaultMissingIntVal;
        if (hourField != (DBObjTableField *) NULL) {
            hour = hourField->Int(record);
            if (hour == hourField->IntNoData()) hour = DBDefaultMissingIntVal;
        }
        else hour = DBDefaultMissingIntVal;
        if (minField != (DBObjTableField *) NULL) {
            min = minField->Int(record);
            if (min == minField->IntNoData()) min = DBDefaultMissingIntVal;
        }
        else min = DBDefaultMissingIntVal;
        date.Set(year, month, day, hour, min);
        dstField->Date(record, date);
    }

    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}
