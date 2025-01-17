/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

DBTableMisc.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <DBif.hpp>

DBTableIF::DBTableIF(DBObjData *data) {
    DataPTR = data;
    ItemTablePTR = data->Table(DBrNItems);
}

DBInt DBTableIF::AppendASCII(char *fileName) {
    DBInt fieldID, fieldNum, intValue, bufferLength, i;
    DBFloat floatValue;
    DBDate dateValue;
    FILE *inFile;
    char buffer[2048], *fieldToken;
    DBObjTableField **fields;
    DBObjRecord *record;

    if ((inFile = fopen(fileName, "r")) == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "File Openning Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }

    if (fgets(buffer, sizeof(buffer) - 1, inFile) == (char *) NULL) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        fclose(inFile);
        return (DBFault);
    }
    bufferLength = strlen(buffer);
    while ((buffer[bufferLength - 1] == '\n') || (buffer[bufferLength - 1] == '\r')) {
        buffer[bufferLength - 1] = '\0';
        bufferLength = strlen(buffer);
    }
    fieldNum = 0;
    fields = (DBObjTableField **) NULL;
    fieldToken = buffer;
    do {
        for (i = 0; fieldToken + i < buffer + bufferLength; ++i) if (fieldToken[i] == DBASCIISeparator) break;
        fieldToken[i] = '\0';
        if (fieldToken[0] == '\"' || fieldToken[0] == '\'') {
            fieldToken[--i] = '\0';
            fieldToken++;
        }
        fields = (DBObjTableField **) realloc(fields, sizeof(DBObjTableField *) * (fieldNum + 1));
        if (fields == (DBObjTableField **) NULL) {
            CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
            fclose(inFile);
            return (DBFault);
        }
        fields[fieldNum] = ItemTablePTR->Field(fieldToken);
        fieldToken = fieldToken + i + 1;
        fieldNum++;
    } while (buffer + bufferLength > fieldToken);

    while (fgets(buffer, sizeof(buffer) - 1, inFile) != (char *) NULL) {
        bufferLength = strlen(buffer);
        while ((buffer[bufferLength - 1] == '\n') || (buffer[bufferLength - 1] == '\r')) {
            buffer[bufferLength - 1] = '\0';
            bufferLength = strlen(buffer);
        }
        record = ItemTablePTR->Add();
        fieldID = 0;
        fieldToken = buffer;
        for (fieldID = 0; fieldID < fieldNum; ++fieldID) {
            for (i = 0; fieldToken + i < buffer + bufferLength; ++i)
                if (fieldToken[i] == DBASCIISeparator) {
                    fieldToken[i] = '\0';
                    break;
                }

            if (i > 0) {
                if (fieldToken[0] == '\"' || fieldToken[0] == '\'') {
                    fieldToken[--i] = '\0';
                    fieldToken++;
                }
            }
            if (i == 0) {
                if ((fieldID < fieldNum) && (fields[fieldID] != (DBObjTableField *) NULL))
                    switch (fields[fieldID]->Type()) {
                        case DBTableFieldString:
                            fields[fieldID]->String(record, "");
                            break;
                        case DBTableFieldInt:
                            fields[fieldID]->Int(record, fields[fieldID]->IntNoData());
                            break;
                        case DBTableFieldFloat:
                            fields[fieldID]->Float(record, fields[fieldID]->FloatNoData());
                            break;
                    }
            }
            else {
                if ((fieldID < fieldNum) && (fields[fieldID] != (DBObjTableField *) NULL))
                    switch (fields[fieldID]->Type()) {
                        case DBTableFieldString:
                            fields[fieldID]->String(record, fieldToken);
                            break;
                        case DBTableFieldInt:
                            if (sscanf(fieldToken, "%d", &intValue) == 1)
                                fields[fieldID]->Int(record, intValue);
                            else fields[fieldID]->Int(record, fields[fieldID]->IntNoData());
                            break;
                        case DBTableFieldFloat:
                            if (sscanf(fieldToken, "%lf", &floatValue) == 1)
                                fields[fieldID]->Float(record, floatValue);
                            else fields[fieldID]->Float(record, fields[fieldID]->FloatNoData());
                            break;
                        case DBTableFieldDate:
                            dateValue.Set(fieldToken);
                            fields[fieldID]->Date(record, dateValue);
                            break;
                    }
            }
            fieldToken = fieldToken + i + 1;
        }
    }
    fclose(inFile);
    free(fields);
    return (DBSuccess);
}
