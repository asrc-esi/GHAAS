/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDtblDeleteRec.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <DBif.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input file> <output file>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -a, --table     [table name]");
    CMmsgPrint(CMmsgInfo, "     -c, --condition [expression]");
    CMmsgPrint(CMmsgInfo, "     -s, --selection");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBInt fromSelection = false, recID;
    char *tableName = (char *) NULL;
    char *expr = (char *) NULL;
    DBInt cellID;
    DBMathOperand *operand;
    DBObjectLIST<DBObject> *variables = new DBObjectLIST<DBObject>("Variables");
    DBObjData  *data;
    DBObjTable *table, *saveTable, *groups = (DBObjTable *) NULL, *saveGroups;
    DBObjRecord *record;
    DBNetworkIF *netIF = (DBNetworkIF *) NULL;

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
        if (CMargTest (argv[argPos], "-c", "--condition")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing expression!");
                return (CMfailed);
            }
            if (expr != (char *) NULL) {
                CMmsgPrint(CMmsgUsrError, "Expression is already set");
                return (CMfailed);
            }
            expr = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-s", "--selection")) {
            fromSelection = true;
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
    if (expr != (char *) NULL) {
        operand = new DBMathOperand(expr);
        if (operand->Expand(variables) == DBFault) return (CMfailed);
    }

    data = new DBObjData();
    if (((argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin)) == DBFault) {
        delete data;
        delete operand;
        return (CMfailed);
    }

    if (tableName == (char *) NULL) tableName = DBrNItems;
    if (strcmp (tableName,DBrNCells) == 0) netIF = new DBNetworkIF (data);

    if ((table = data->Table(tableName)) == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Invalid table!");
        delete data;
        delete operand;
        return (CMfailed);
    }

    saveTable = new DBObjTable(*table);
    if (netIF != (DBNetworkIF *) NULL) {
        for (cellID = 0; cellID < table->ItemNum(); ++cellID) {
            record = table->Item(cellID);
            netIF->CellDelete(record);
        }
    }
    table->DeleteAll();
    if ((strcmp(tableName, DBrNItems) == 0) && ((groups = data->Table(DBrNGroups)) != (DBObjTable *) NULL)) {
        saveGroups = new DBObjTable(*groups);
        groups->DeleteAll();
    }

    if (expr != (char *) NULL) {
        if (operand->Configure(saveTable->Fields()) == DBFault) {
            delete data;
            delete operand;
            return (CMfailed);
        }

        for (recID = 0; recID < saveTable->ItemNum(); ++recID) {
            record = saveTable->Item(recID);
            if (operand->Int(record) == true) {
                if (!fromSelection) continue;
                if ((record->Flags() & DBObjectFlagSelected) == DBObjectFlagSelected) continue;
            }
            table->Add(new DBObjRecord(*record));
            if (groups != (DBObjTable *) NULL) groups->Add(new DBObjRecord(*(saveGroups->Item(recID))));
        }
    }
    else {
        if (fromSelection) {
            for (recID = 0; recID < saveTable->ItemNum(); ++recID) {
                record = saveTable->Item(recID);
                if ((record->Flags() & DBObjectFlagSelected) == DBObjectFlagSelected) {
                    table->Add(new DBObjRecord(*record));
                    if (groups != (DBObjTable *) NULL)
                        groups->Add(new DBObjRecord(*(saveGroups->Item(recID))));
                }
            }
        }
    }
    if (netIF != (DBNetworkIF *) NULL) {
        for (cellID = 0; cellID < netIF->CellNum(); ++cellID) {
            record = table->Item(cellID);
            netIF->CellAdd(record);
        }
    }

    if (expr != (char *) NULL) delete operand;
    delete variables;
    delete saveTable;
    if (groups != (DBObjTable *) NULL) delete saveGroups;

    if (netIF != (DBNetworkIF *) NULL) {
        netIF->Trim();
        netIF->Build();
        delete netIF;
    }
    ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);
    delete data;
    return (ret);
}
