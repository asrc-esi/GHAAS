/******************************************************************************

GHAAS RiverGIS Libarary V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

RGlibGenFuncs.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <RG.hpp>

DBInt RGlibGenFuncFieldCompare(DBObjTable *table, char *f0Text, char *f1Text, char *rText, DBInt diffMethod) {
    DBInt intVal, ret, i, rowID;
    DBFloat val[2];
    DBObjTableField *field[2];
    DBObjTableField *result;
    DBObjRecord *record;

    field[0] = table->Field(f0Text);
    field[1] = table->Field(f1Text);
    result = table->Field(rText);
    if ((field[0] == (DBObjTableField *) NULL) || (field[1] == (DBObjTableField *) NULL)) {
        CMmsgPrint(CMmsgAppError, "Invalid Compare Fields in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (result == (DBObjTableField *) NULL)
        table->AddField(result = new DBObjTableField(rText, DBTableFieldFloat, "%10.3f", sizeof(DBFloat4)));
    if (diffMethod > 0) result->Format("%6.2f");
    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        if ((record->Flags() & DBObjectFlagIdle) == DBObjectFlagIdle)
            result->Float(record, result->FloatNoData());
        else {
            for (i = 0; i < 2; ++i)
                if (field[i]->Type() == DBTableFieldInt) {
                    intVal = field[i]->Int(record);
                    ret = intVal == field[i]->IntNoData();
                    val[i] = (DBFloat) intVal;
                }
                else {
                    val[i] = field[i]->Float(record);
                    ret = CMmathEqualValues(val[i], field[i]->FloatNoData());
                }
            if (ret) result->Float(record, result->FloatNoData());
            else
                switch (diffMethod) {
                    default:
                        result->Float(record, val[0] - val[1]);
                        break;
                    case 1:
                        result->Float(record, (val[0] - val[1]) * 100.0 / val[0]);
                        break;
                    case 2:
                        result->Float(record, (val[0] - val[1]) * 100.0 / (val[0] > val[1] ? val[0] : val[1]));
                        break;
                    case 3:
                        result->Float(record, fabs((val[0] - val[1]) * 100.0 / (val[0] > val[1] ? val[0] : val[1])));
                        break;
                }
        }
    }
    return (DBSuccess);
}

DBInt RGlibGenFuncFieldCalculate(DBObjTable *table, char *f0Text, char *f1Text, char *rText, DBInt oper) {
    DBInt intVal, ret, i, rowID;
    DBFloat val[2];
    DBObjTableField *field[2];
    DBObjTableField *result;
    DBObjRecord *record;

    field[0] = table->Field(f0Text);
    field[1] = table->Field(f1Text);
    result = table->Field(rText);

    if ((field[0] == (DBObjTableField *) NULL) && (sscanf(f0Text, "%lf", val) != 1)) return (DBFault);
    if ((field[1] == (DBObjTableField *) NULL) && (sscanf(f1Text, "%lf", val + 1) != 1)) return (DBFault);

    if (result == (DBObjTableField *) NULL)
        table->AddField(result = new DBObjTableField(rText, DBTableFieldFloat, "%10.3f", sizeof(DBFloat4)));
    if (field[0] != (DBObjTableField *) NULL)
        result->Format(field[0]->Format());
    else if (field[1] != (DBObjTableField *) NULL)
        result->Format(field[1]->Format());

    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        if ((record->Flags() & DBObjectFlagIdle) == DBObjectFlagIdle) continue;
        else {
            for (i = 0; i < 2; ++i)
                if (field[i] != (DBObjTableField *) NULL) {
                    if (field[i]->Type() == DBTableFieldInt) {
                        intVal = field[i]->Int(record);
                        val[i] = (DBFloat) intVal;
                        ret = intVal == field[i]->IntNoData();
                    }
                    else {
                        val[i] = field[i]->Float(record);
                        ret = CMmathEqualValues(val[i], field[i]->FloatNoData());
                    }
                    if (ret) {
                        result->Float(record, result->FloatNoData());
                        continue;
                    }
                }
            if ((oper == DBMathOperatorDiv) && (val[1] == 0))
                result->Float(record, result->FloatNoData());
            else
                switch (oper) {
                    case DBMathOperatorAdd:
                        result->Float(record, val[0] + val[1]);
                        break;
                    case DBMathOperatorSub:
                        result->Float(record, val[0] - val[1]);
                        break;
                    case DBMathOperatorMul:
                        result->Float(record, val[0] * val[1]);
                        break;
                    case DBMathOperatorDiv:
                        result->Float(record, val[0] / val[1]);
                        break;
                    default:
                        CMmsgPrint(CMmsgAppError, "Invalid Operand in: %s %d", __FILE__, __LINE__);
                        break;
                }
        }
    }
    return (DBSuccess);
}

DBInt RGlibGenFuncTopoAccum(DBObjTable *table, char *nextStnFldName, char *srcFldName, char *dstFldName) {
    DBInt rowID;
    DBObjTableField *srcField, *dstField, *nextField;
    DBObjRecord *record, *nextRec;

    if (((srcField = table->Field(srcFldName)) == (DBObjTableField *) NULL) ||
        ((nextField = table->Field(nextStnFldName)) == (DBObjTableField *) NULL))
        return (DBFault);
    if ((dstField = table->Field(dstFldName)) == (DBObjTableField *) NULL) {
        dstField = new DBObjTableField(*srcField);
        dstField->Name(dstFldName);
        table->AddField(dstField);
    }
    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        dstField->Float(record, srcField->Float(record));
        record->Flags(DBObjectFlagSelected, DBClear);
    }
    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        DBPause(rowID * 100 / table->ItemNum());
        if ((record->Flags() & DBObjectFlagIdle) == DBObjectFlagIdle) {
            dstField->Float(record, dstField->FloatNoData());
            continue;
        }
        for (nextRec = table->Item(nextField->Int(record) - 1);
             nextRec != (DBObjRecord *) NULL;
             nextRec = table->Item(nextField->Int(nextRec) - 1)) {
            if (nextRec == record) {
                record->Flags(DBObjectFlagSelected, DBSet);
                break;
            }
            if ((nextRec->Flags() & DBObjectFlagIdle) != DBObjectFlagIdle) {
                if (CMmathEqualValues(dstField->Float(nextRec), dstField->FloatNoData()) ||
                    CMmathEqualValues(srcField->Float(record), srcField->FloatNoData()))
                    dstField->Float(nextRec, dstField->FloatNoData());
                else
                    dstField->Float(nextRec, dstField->Float(nextRec) + srcField->Float(record));
            }
        }
    }
    return (DBSuccess);
}

DBInt RGlibGenFuncTopoSubtract(DBObjTable *table, char *nextStnFldName, char *srcFldName, char *dstFldName) {
    DBInt rowID;
    DBObjTableField *srcField, *dstField, *nextField;
    DBObjRecord *record, *nextRec;

    if (((srcField = table->Field(srcFldName)) == (DBObjTableField *) NULL) ||
        ((nextField = table->Field(nextStnFldName)) == (DBObjTableField *) NULL))
        return (DBFault);
    if ((dstField = table->Field(dstFldName)) == (DBObjTableField *) NULL) {
        dstField = new DBObjTableField(*srcField);
        dstField->Name(dstFldName);
        table->AddField(dstField);
    }
    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        dstField->Float(record, srcField->Float(record));
    }
    for (rowID = 0; rowID < table->ItemNum(); ++rowID) {
        record = table->Item(rowID);
        DBPause(rowID * 100 / table->ItemNum());
        if ((record->Flags() & DBObjectFlagIdle) == DBObjectFlagIdle) {
            dstField->Float(record, dstField->FloatNoData());
            continue;
        }
        for (nextRec = table->Item(nextField->Int(record) - 1);
             nextRec != (DBObjRecord *) NULL;
             nextRec = table->Item(nextField->Int(nextRec) - 1))
            if ((nextRec->Flags() & DBObjectFlagIdle) != DBObjectFlagIdle) {
                if (CMmathEqualValues(dstField->Float(nextRec), dstField->FloatNoData()) ||
                    CMmathEqualValues(srcField->Float(record), srcField->FloatNoData()))
                    dstField->Float(nextRec, dstField->FloatNoData());
                else
                    dstField->Float(nextRec, dstField->Float(nextRec) - srcField->Float(record));
                break;
            }
    }
    return (DBSuccess);
}

int RGlibGenFuncSymbolField(DBObjData *data, const char *fieldName) {
    DBInt recID;
    char symbolName[DBStringLength + 1];
    DBObjTable *table = data->Table(DBrNItems);
    DBObjTable *symbols = data->Table(DBrNSymbols);
    DBObjRecord *record, *symbolRec;
    DBObjTableField *field;
    DBObjTableField *symbolFLD;
    DBObjTableField *symbolIDFLD;
    DBObjTableField *foregroundFLD;
    DBObjTableField *backgroundFLD;
    DBObjTableField *styleFLD;

    if (table == (DBObjTable *) NULL) return (DBFault);
    if ((field = table->Field(fieldName)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Invalid field name in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if ((symbolFLD = table->Field(DBrNSymbol)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing symbol field in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (symbols == (DBObjTable *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing symbol table in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if ((symbolIDFLD = symbols->Field(DBrNSymbolID)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing symbolID field in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if ((foregroundFLD = symbols->Field(DBrNForeground)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing foreground field in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if ((backgroundFLD = symbols->Field(DBrNBackground)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing background field in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if ((styleFLD = symbols->Field(DBrNStyle)) == (DBObjTableField *) NULL) {
        CMmsgPrint(CMmsgAppError, "Missing style field in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }

    symbols->DeleteAll();
    for (recID = 0; recID < table->ItemNum(); ++recID) {
        record = table->Item(recID);
        DBPause(record->RowID() * 100 / table->ItemNum());

        if (field->Type() == DBTableFieldString) snprintf(symbolName, sizeof(symbolName), "%s", field->String(record));
        else snprintf(symbolName, sizeof(symbolName), "Symbol:%03d", field->Int(record));
        if ((symbolRec = (DBObjRecord *) symbols->Item(symbolName)) == (DBObjRecord *) NULL) {
            if ((symbolRec = symbols->Add(symbolName)) == NULL) {
                CMmsgPrint(CMmsgAppError, "Symbol Object Creation Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
            symbolIDFLD->Int(symbolRec, field->Type() == DBTableFieldString ? symbolRec->RowID() : field->Int(record));
            foregroundFLD->Int(symbolRec, 1);
            backgroundFLD->Int(symbolRec, 0);
            styleFLD->Int(symbolRec, 0);
        }
        symbolFLD->Record(record, symbolRec);
    }
    return (DBSuccess);
}
