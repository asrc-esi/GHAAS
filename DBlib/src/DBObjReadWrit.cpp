/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

DBObjReadWrit.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <time.h>
#include <pwd.h>

int DBVarString::Read(FILE *file, int swap) {
    if (fread(&LengthVAR, sizeof(LengthVAR), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (swap) Swap();
    if ((StringPTR = (DBAddress) ((char *) malloc(LengthVAR + 1) - (char *) NULL)) == (DBAddress) NULL) {
        CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (fread((char *) NULL + StringPTR, LengthVAR + 1, 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBVarString::Write(FILE *file) {
    if (fwrite(&LengthVAR, sizeof(LengthVAR), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (fwrite((char *) NULL + StringPTR, LengthVAR + 1, 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBObject::Read(FILE *file, int swap) {
    if (NameSTR.Read(file, swap) != DBSuccess) return (DBFault);
    if (fread((char *) this + sizeof(DBObjectHeader), sizeof(DBObject) - sizeof(DBObjectHeader) - sizeof(NameSTR), 1,
              file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (swap) Swap();
    return (DBSuccess);
}

int DBObject::Write(FILE *file) {
    if (NameSTR.Write(file) != DBSuccess) return (DBFault);
    if (fwrite((char *) this + sizeof(DBObjectHeader), sizeof(DBObject) - sizeof(DBObjectHeader) - sizeof(NameSTR), 1,
               file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBObjRecord::Read(FILE *file, int swap) {
    if (DBObject::Read(file, swap) != DBSuccess) return (DBFault);

    if (fread((char *) this + sizeof(DBObject), sizeof(DBObjRecord) - sizeof(DBObject) - sizeof(DBAddress), 1, file) !=
        1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (swap) Swap();

    if ((DataPTR = (DBAddress) ((char *) malloc(Length()) - (char *) NULL)) == (DBAddress) NULL) {
        CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (fread((char *) NULL + DataPTR, Length(), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (swap && ElementSize () > 1) {
        DBUnsigned i;
        void (*swapFunc)(void *);

        switch (ElementSize()) {
            case 2:
                swapFunc = DBByteOrderSwapHalfWord;
                break;
            case 4:
                swapFunc = DBByteOrderSwapWord;
                break;
            case 8:
                swapFunc = DBByteOrderSwapLongWord;
                break;
            case sizeof (DBCoordinate):
                swapFunc = DBByteOrderSwapCoordinate;
                break;
            default:
                CMmsgPrint(CMmsgAppError, "Invalid Item Size (Record %d %s) in: %s %d", RowID(), Name(), __FILE__,
                           __LINE__);
                return (DBFault);
        }
        for (i = 0; i < Length(); i += ElementSize()) (*swapFunc)((char *) NULL + DataPTR + i * ElementSize());
    }
    return (DBSuccess);
}

int DBObjRecord::Write(FILE *file) {
    if (DBObject::Write(file) != DBSuccess) return (DBFault);

    if (fwrite((char *) this + sizeof(DBObject), sizeof(DBObjRecord) - sizeof(DBObject) - sizeof(DBAddress),1, file) !=
        1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (fwrite((char *) NULL + DataPTR, Length(), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBObjTableField::Read(FILE *file, int swap) {
    if (DBObject::Read(file, swap) != DBSuccess) return (DBFault);
    if (fread((char *) this + sizeof(DBObject), sizeof(DBObjTableField) - sizeof(DBObject), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (swap) Swap();
    if ((strcmp(Name(), "Drain") == 0) && Required()) Required(false);
    return (DBSuccess);
}

int DBObjTableField::Write(FILE *file) {
    if (DBObject::Write(file) != DBSuccess) return (DBFault);
    if (fwrite((char *) this + sizeof(DBObject), sizeof(DBObjTableField) - sizeof(DBObject), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBObjTable::Read(FILE *file, int swap) {
    DBInt id;
    DBDate date;
    DBCoordinate coord;
    DBRegion region;
    DBPosition pos;
    DBUShort row, col;
    DBObjRecord *record;
    DBObjTableField *field;

    if (DBObjectLIST<DBObjRecord>::Read(file, swap) != DBSuccess) return (DBFault);
    if (FieldPTR->Read(file, swap) != DBSuccess) return (DBFault);
    for (id = 0; id < FieldPTR->ItemNum(); ++id)
        if (FieldPTR->ReadItem(file, id, swap) == DBFault) return (DBFault);

    RecordLengthVAR = 0;
    for (field = FieldPTR->First(); field != (DBObjTableField *) NULL; field = FieldPTR->Next())
        RecordLengthVAR = RecordLengthVAR > field->StartByte() + field->Length() ?
                          RecordLengthVAR : field->StartByte() + field->Length();

    for (id = 0; id < ItemNum(); ++id) {
        if (ReadItem(file, id, swap) == DBFault) return (DBFault);
        record = Item(id);
        if (swap == false) continue;
        for (field = FieldPTR->First(); field != (DBObjTableField *) NULL; field = FieldPTR->Next())
            switch (field->Type()) {
                case DBTableFieldInt:
                case DBTableFieldFloat:
                case DBTableFieldTableRec:
                case DBTableFieldDataRec:
                    switch (field->Length()) {
                        case 2:
                            DBByteOrderSwapHalfWord((char *) record->Data() + field->StartByte());
                            break;
                        case 4:
                            DBByteOrderSwapWord((char *) record->Data() + field->StartByte());
                            break;
                        case 8:
                            DBByteOrderSwapLongWord((char *) record->Data() + field->StartByte());
                            break;
                    }
                    break;
                case DBTableFieldDate:
                    date = field->Date(record);
                    date.Swap();
                    field->Date(record, date);
                    break;
                case DBTableFieldCoord:
                    coord = field->Coordinate(record);
                    coord.Swap();
                    field->Coordinate(record, coord);
                    break;
                case DBTableFieldRegion:
                    region = field->Region(record);
                    region.Swap();
                    field->Region(record, region);
                    break;
                case DBTableFieldPosition:
                    pos = field->Position(record);
                    switch (field->Length()) {
                        case sizeof(DBPosition):
                            pos.Swap();
                            break;
                        default:
                            row = pos.Row;
                            col = pos.Col;
                            DBByteOrderSwapHalfWord(&row);
                            DBByteOrderSwapHalfWord(&col);
                            pos.Row = row;
                            pos.Col = col;
                            break;
                    }
                    field->Position(record, pos);
                    break;
                default:
                    break;
            }
    }
    if (MethodPTR->Read(file, swap) != DBSuccess) return (DBFault);
    for (id = 0; id < MethodPTR->ItemNum(); ++id) if (MethodPTR->ReadItem(file, id, swap) == DBFault) return (DBFault);
    return (DBSuccess);
}

int DBObjTable::Write(FILE *file) {
    DBInt id;
    DBObjRecord *obj;
    DBObjRecord *record;
    DBObjTableField *field;

    if (DBObjectLIST<DBObjRecord>::Write(file) != DBSuccess) return (DBFault);
    if (FieldPTR->Write(file) != DBSuccess) return (DBFault);
    for (id = 0; id < FieldPTR->ItemNum(); ++id)
        if (FieldPTR->WriteItem(file, id) == DBFault) return (DBFault);

    for (id = 0; id < ItemNum(); ++id) {
        record = Item(id);
        for (field = FieldPTR->First(); field != (DBObjTableField *) NULL; field = FieldPTR->Next())
            switch (field->Type()) {
                case DBTableFieldTableRec:
                case DBTableFieldDataRec:
                    if ((obj = field->Record(record)) != (DBObjRecord *) NULL)
                         field->Record(record, (DBObjRecord *) ((char *) NULL + obj->RowID()));
                    else field->Record(record, (DBObjRecord *) DBFault);
                    break;
            }
        if (WriteItem(file, id) == DBFault) return (DBFault);
    }
    if (MethodPTR->Write(file) != DBSuccess) return (DBFault);
    for (id = 0; id < MethodPTR->ItemNum(); ++id) if (MethodPTR->WriteItem(file, id) == DBFault) return (DBFault);
    return (DBSuccess);
}

int DBDataHeader::Read(FILE *file) {
    if (fread(this, sizeof(DBDataHeader), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    if (ByteOrderVAR != 1) {
        Swap();
        return (true);
    }
    return (false);
}

int DBDataHeader::Write(FILE *file) {
    time_t curTime = time (NULL);
    struct tm *tmStruct = localtime (&curTime);
    LastModVAR.Set (tmStruct->tm_year + 1900,tmStruct->tm_mon,tmStruct->tm_mday - 1,tmStruct->tm_hour, tmStruct->tm_min);
    MajorVAR = 2;
    MinorVAR = 2;
    if (fwrite(this, sizeof(DBDataHeader), 1, file) != 1) {
        CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    return (DBSuccess);
}

int DBObjData::Read(const char *fileName) {
    DBInt ret;
    FILE *file;

    if (strncmp(CMfileExtension(fileName), "nc", 2) == 0) {
        Type(DBTypeGridContinuous); // TODO: Limiting to Continuous grid
        ret = DBImportNetCDF(this, fileName);
    }
    else if (strncmp(CMfileExtension(fileName), "gz", 2) == 0) {
        char pCommand[strlen(fileName) + 16];
        snprintf(pCommand, sizeof(pCommand), "gunzip -c %s", fileName);
        if ((file = popen(pCommand, "r")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgSysError, "File (%s) Opening Error in: %s %d", fileName, __FILE__, __LINE__);
            return (DBFault);
        }
        ret = Read(file);
        pclose(file);
    }
    else {
        if ((file = fopen(fileName, "r")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgAppError, "File (%s) Opening Error in: %s %d", fileName, __FILE__, __LINE__);
            return (DBFault);
        }
        ret = Read(file);
        fclose(file);
    }
    FileName(fileName);
    return (ret);
}

int DBObjData::Read(FILE *file) {
    int swap;

    DocsPTR->DeleteAll();
    ArraysPTR->DeleteAll();
    TablesPTR->DeleteAll();
    if ((swap = DBDataHeader::Read(file)) == DBFault) return (DBFault);
    return (_Read(file, swap));
}

int DBObjData::_Read(FILE *file, int swap) {
    DBInt id;
    DBObjRecord *docRec;

    if (DBObject::Read(file, swap) == DBFault) return (DBFault);
    if (DocsPTR->Read(file, swap) == DBFault) return (DBFault);
    for (id = 0; id < DocsPTR->ItemNum(); ++id) {
        if (DocsPTR->ReadItem(file, id, swap) == DBFault) return (DBFault);
        docRec = DocsPTR->Item(id);
        if (((DBVarString *) docRec->Data())->Read(file, swap) == DBFault) return (DBFault);
    }
    if (ArraysPTR->Read(file, swap) == DBFault) return (DBFault);
    for (id = 0; id < ArraysPTR->ItemNum(); ++id)
        if (ArraysPTR->ReadItem(file, id, swap) == DBFault) return (DBFault);
    TablesPTR->Read(file, swap);
    for (id = 0; id < TablesPTR->ItemNum(); ++id)
        if (TablesPTR->ReadItem(file, id, swap) == DBFault) return (DBFault);
    return (BuildFields());
}

int DBObjData::Write(const char *fileName) {
    DBInt ret;
    FILE *file;

    if (strncmp(CMfileExtension(fileName), "nc", 2) == 0)
        ret = DBExportNetCDF(this, fileName);
    else if (strncmp(CMfileExtension(fileName), "gz", 2) == 0) {
        char pCommand[strlen(fileName) + 16];
        snprintf(pCommand, sizeof(pCommand),"gzip > %s", fileName);
        if ((file = popen(pCommand, "w")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgSysError, "File (%s) Opening Error in: %s %d", fileName, __FILE__, __LINE__);
            return (DBFault);
        }
        ret = Write(file);
        pclose(file);
    }
    else {
        if ((file = fopen(fileName, "w")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgSysError, "File (%s) Opening Error in: %s %d", fileName, __FILE__, __LINE__);
            return (DBFault);
        }
        ret = Write(file);
        fclose(file);
    }
    return (ret);
}

int DBObjData::Write(FILE *file) {
    RecalcExtent();
    if (DBDataHeader::Write(file) == DBFault) return (DBFault);
    return (_Write(file));
}

int DBObjData::_Write(FILE *file) {
    DBInt id, userFlags;
    DBObjRecord *docRec;
    struct passwd *pwd = getpwuid(geteuid());

    if ((pwd != (struct passwd *) NULL) && (pwd->pw_gecos != (char *) NULL) && (strlen (pwd->pw_gecos) > 0)) {
        Document(DBDocOwnerPerson, pwd->pw_gecos);
    }
    userFlags = Flags() & DBDataFlagUserModeFlags;
    Flags(DBDataFlagUserModeFlags, DBClear);
    if (DBObject::Write(file) == DBFault) return (DBFault);
    Flags(userFlags, DBSet);
    if (DocsPTR->Write(file) == DBFault) return (DBFault);
    for (id = 0; id < DocsPTR->ItemNum(); ++id) {
        if (DocsPTR->WriteItem(file, id) == DBFault) return (DBFault);
        docRec = DocsPTR->Item(id);
        if (((DBVarString *) docRec->Data())->Write(file) == DBFault) return (DBFault);
    }
    if (ArraysPTR->Write(file) == DBFault) return (DBFault);
    for (id = 0; id < ArraysPTR->ItemNum(); ++id)
        if (ArraysPTR->WriteItem(file, id) == DBFault) return (DBFault);
    TablesPTR->Write(file);
    for (id = 0; id < TablesPTR->ItemNum(); ++id)
        if (TablesPTR->WriteItem(file, id) == DBFault) return (DBFault);
    return (BuildFields());
}

int DBObjData::BuildFields() {
    DBInt ret = DBSuccess;
    DBObjTable *table, *refTable;
    DBObjRecord *tableRec;
    DBObjTableField *field;
    DBObjectLIST<DBObjTableField> *tableFLDs;

    for (table = TablesPTR->First(); table != (DBObjTable *) NULL; table = TablesPTR->Next()) {
        tableFLDs = table->Fields();
        for (field = tableFLDs->First(); field != (DBObjTableField *) NULL; field = tableFLDs->Next())
            switch (field->Type()) {
                case DBTableFieldTableRec:
                    if ((refTable = TablesPTR->Item(field->RecordProp())) == (DBObjTable *) NULL) {
                        CMmsgPrint(CMmsgAppError, "Corrupt Dataset in: %s %d", __FILE__, __LINE__);
                        ret = DBFault;
                        continue;
                    }
                    for (tableRec = table->First(); tableRec != (DBObjRecord *) NULL; tableRec = table->Next())
                        if ((DBInt) (field->Record(tableRec) - (DBObjRecord *) NULL) != DBFault)
                            field->Record(tableRec,
                                          refTable->Item((DBInt) ((char *) (field->Record(tableRec)) - (char *) NULL)));
                        else field->Record(tableRec, (DBObjRecord *) NULL);
                    break;
                case DBTableFieldDataRec:
                    for (tableRec = table->First(); tableRec != (DBObjRecord *) NULL; tableRec = table->Next())
                        if ((DBInt) (field->Record(tableRec) - (DBObjRecord *) NULL) != DBFault)
                            field->Record(tableRec, ArraysPTR->Item(
                                    (DBInt) ((char *) (field->Record(tableRec)) - (char *) NULL)));
                        else field->Record(tableRec, (DBObjRecord *) NULL);
                    break;
            }
    }
    return (ret);
}
