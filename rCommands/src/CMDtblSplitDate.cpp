/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblSplitDate.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static char *_CMDpadit(int number, bool isYear);

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -Y, --year      [year field]");
    CMmsgPrint(CMmsgInfo, "     -M, --month     [month field]");
    CMmsgPrint(CMmsgInfo, "     -D, --day       [day field]");
    CMmsgPrint(CMmsgInfo, "     -H, --hour      [hour field]");
    CMmsgPrint(CMmsgInfo, "     -I, --minute    [minute field]");
    CMmsgPrint(CMmsgInfo, "     -p, --padding");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt recID;
    bool padding = false;
    char *tableName = (char *) NULL;
    char *fieldName = (char *) NULL;
    char *yearFieldName = (char *) NULL;
    char *monthFieldName = (char *) NULL;
    char *dayFieldName = (char *) NULL;
    char *hourFieldName = (char *) NULL;
    char *minFieldName = (char *) NULL;
    char *tmp = (char *) NULL;
    DBObjData *data;
    DBObjTable *table;
    DBObjTableField *srcField;
    DBObjTableField *yearField = (DBObjTableField *) NULL;
    DBObjTableField *monthField = (DBObjTableField *) NULL;
    DBObjTableField *dayField = (DBObjTableField *) NULL;
    DBObjTableField *hourField = (DBObjTableField *) NULL;
    DBObjTableField *minField = (DBObjTableField *) NULL;
    DBInt dbInputType = DBTableFieldInt;
    DBInt dbInputSize = sizeof(DBShort);
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
        if (CMargTest (argv[argPos], "-p", "--padding")) {
            padding = true;
            dbInputType = DBTableFieldString;
            dbInputSize = sizeof(DBByte) * 3;
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

    if (fieldName == (char *) NULL) fieldName = (char *) "Date";
    if ((srcField = table->Field(fieldName)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing date field!");
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }
    if ((srcField->Type() != DBTableFieldString) &&
        (srcField->Type() != DBTableFieldDate)) {
        CMmsgPrint(CMmsgUsrError, "Invalid date field!");
        _CMDprintUsage (argv[0]);
        delete data;
        return (CMfailed);
    }

    if (yearFieldName != (char *) NULL) {
        if ((yearField = table->Field(yearFieldName)) == (DBObjTableField *) NULL) {
            yearField = new DBObjTableField(yearFieldName, DBTableFieldInt, "%4d", sizeof(DBShort), false);
            table->AddField(yearField);
        }
    }

    if (monthFieldName != (char *) NULL) {
        if ((monthField = table->Field(monthFieldName)) == (DBObjTableField *) NULL) {
            monthField = new DBObjTableField(monthFieldName, dbInputType, "%2d", dbInputSize, false);
            table->AddField(monthField);
        }
    }

    if (dayFieldName != (char *) NULL) {
        if (monthField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Month field is not set!");
            delete data;
            return (CMfailed);
        }
        if ((dayField = table->Field(dayFieldName)) == (DBObjTableField *) NULL) {
            dayField = new DBObjTableField(dayFieldName, dbInputType, "%2d", dbInputSize, false);
            table->AddField(dayField);
        }
    }

    if (hourFieldName != (char *) NULL) {
        if (dayField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Day field is not set!");
            delete data;
            return (CMfailed);
        }
        if ((hourField = table->Field(hourFieldName)) == (DBObjTableField *) NULL) {
            hourField = new DBObjTableField(hourFieldName, dbInputType, "%2d", dbInputSize, false);
            table->AddField(hourField);
        }
    }

    if (minFieldName != (char *) NULL) {
        if (hourField == (DBObjTableField *) NULL) {
            CMmsgPrint(CMmsgUsrError, "Hour field is not set!");
            delete data;
            return (CMfailed);
        }
        if ((minField = table->Field(minFieldName)) == (DBObjTableField *) NULL) {
            minField = new DBObjTableField(minFieldName, dbInputType, "%2d", dbInputSize, false);
            table->AddField(minField);
        }
    }

    for (recID = 0; recID < table->ItemNum(); ++recID) {
        record = table->Item(recID);
        if (srcField->Type() == DBTableFieldString)
            date.Set(srcField->String(record));
        else date = srcField->Date(record);
        if (yearField != (DBObjTableField *) NULL) yearField->Int(record, date.Year());
        if (padding) {
            if (monthField != (DBObjTableField *) NULL) {
                if (date.Month() != DBDefaultMissingIntVal)
                    monthField->String(record, tmp = _CMDpadit(date.Month() + 1, false));
                else monthField->String(record, tmp = _CMDpadit(date.Month(), false));
                free(tmp);
            }
            if (dayField != (DBObjTableField *) NULL) {
                dayField->String(record, tmp = _CMDpadit(date.Day(), false));
                free(tmp);
            }
            if (hourField != (DBObjTableField *) NULL) {
                hourField->String(record, tmp = _CMDpadit(date.Hour(), false));
                free(tmp);
            }
            if (minField != (DBObjTableField *) NULL) {
                minField->String(record, tmp = _CMDpadit(date.Minute(), false));
                free(tmp);
            }
        }
        else {
            if (monthField != (DBObjTableField *) NULL) {
                if (date.Month() != DBDefaultMissingIntVal) monthField->Int(record, date.Month() + 1);
                else monthField->Int(record, DBDefaultMissingIntVal);
            }
            if (dayField != (DBObjTableField *) NULL) dayField->Int(record, date.Day());
            if (hourField != (DBObjTableField *) NULL) hourField->Int(record, date.Hour());
            if (minField != (DBObjTableField *) NULL) minField->Int(record, date.Minute());
        }
    }
    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    return (ret);
}

static char *_CMDpadit(int number, bool isYear) {
    char *tmp = (char *) NULL;

    if (isYear) {
        tmp = (char *) malloc(sizeof(char) * 5);
        snprintf(tmp, sizeof(char) * 5, "%04d", number);
    }
    else {
        tmp = (char *) malloc(sizeof(char) * 3);
        snprintf(tmp, sizeof(char) * 3, "%02d", number);
    }
    return tmp;
}
