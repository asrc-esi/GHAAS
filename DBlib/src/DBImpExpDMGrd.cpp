/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

DBImpExpDMGrd.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <DBif.hpp>

#define DMFloat    0
#define DMInt        1
#define DMByte        2

#define DMMatrix    0
#define DMTabular    1
#define DMPoint    2

class DMFileHeader {
private:
    char Id[5];
    int ByteOrderVAR;
    int DataTypeVAR;
    int FileTypeVAR;
    int LayerNumVAR;
    float NorthVAR, SouthVAR, EastVAR, WestVAR;
    float CellHeightVAR, CellWidthVAR;
    int RowNumVAR, ColNumVAR;
    int DataPointNumVAR;
    int MissingValueVAR;
    char FutureExpansion[32];

    void SetExpansion(const char *str) {
        strcpy(FutureExpansion, str);
    }

    void Swap() {
        DBByteOrderSwapWord(&ByteOrderVAR);
        DBByteOrderSwapWord(&DataTypeVAR);
        DBByteOrderSwapWord(&FileTypeVAR);
        DBByteOrderSwapWord(&LayerNumVAR);
        DBByteOrderSwapWord(&NorthVAR);
        DBByteOrderSwapWord(&SouthVAR);
        DBByteOrderSwapWord(&EastVAR);
        DBByteOrderSwapWord(&WestVAR);
        DBByteOrderSwapWord(&CellHeightVAR);
        DBByteOrderSwapWord(&CellWidthVAR);
        DBByteOrderSwapWord(&RowNumVAR);
        DBByteOrderSwapWord(&ColNumVAR);
        DBByteOrderSwapWord(&DataPointNumVAR);
        DBByteOrderSwapWord(&MissingValueVAR);
    }

public:
    DMFileHeader() {
        strcpy(Id, "DM2.0");
        ByteOrderVAR = 1;
        FileTypeVAR = DMMatrix;
        LayerNumVAR = 0;
        NorthVAR = -DBHugeVal;
        SouthVAR = DBHugeVal;
        EastVAR = -DBHugeVal;
        WestVAR = DBHugeVal;
        CellHeightVAR = 0.0;
        CellWidthVAR = 0.0;
        RowNumVAR = ColNumVAR = DataPointNumVAR = 0;
        MissingValueVAR = DBDefaultMissingIntVal;
    }

    int Read(FILE *file) {
        if (fread(this, sizeof(DMFileHeader), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "Reading Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        if (ByteOrderVAR != 1) {
            Swap();
            return (true);
        }
        return (false);
    }

    int Write(FILE *file) {
        if (fwrite(this, sizeof(DMFileHeader), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "Writing Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        return (DBSuccess);
    }

    DBInt DataType() const { return (DataTypeVAR); }

    void DataType(DBInt dataType) { DataTypeVAR = dataType; }

    DBInt FileType() const { return (FileTypeVAR); }

    DBInt LayerNum() const { return (LayerNumVAR); }

    void LayerNum(DBInt layerNum) { LayerNumVAR = layerNum; }

    DBRegion Extent() const {
        DBCoordinate coord;
        DBRegion extent;

        coord.X = EastVAR + CellWidthVAR / 2.0;
        coord.Y = NorthVAR + CellHeightVAR / 2.0;
        extent.Expand(coord);
        coord.X = WestVAR - CellWidthVAR / 2.0;
        coord.Y = SouthVAR - CellHeightVAR / 2.0;
        extent.Expand(coord);
        return (extent);
    }

    void Extent(DBRegion extent) {
        EastVAR = extent.UpperRight.X + CellWidthVAR / 2.0;
        NorthVAR = extent.UpperRight.Y + CellHeightVAR / 2.0;
        WestVAR = extent.LowerLeft.X + CellWidthVAR / 2.0;
        SouthVAR = extent.LowerLeft.Y + CellHeightVAR / 2.0;
    }

    DBFloat CellHeight() const { return (CellHeightVAR); }

    void CellHeight(DBFloat cellHeight) { CellHeightVAR = cellHeight; }

    DBFloat CellWidth() const { return (CellWidthVAR); }

    void CellWidth(DBFloat cellWidth) { CellWidthVAR = cellWidth; }

    DBInt RowNum() const { return (RowNumVAR); }

    void RowNum(DBInt rowNum) {
        RowNumVAR = rowNum;
        DataPointNumVAR = RowNumVAR * ColNumVAR;
    }

    DBInt ColNum() const { return (ColNumVAR); }

    void ColNum(DBInt colNum) {
        ColNumVAR = colNum;
        DataPointNumVAR = RowNumVAR * ColNumVAR;
    }

    DBInt DataPointNum() const { return (DataPointNumVAR); }

    DBFloat MissingValue() const { return ((DBFloat) -9999.0); }
};

class DMLayerHeader {
private:
    char DescriptionSTR[60];
    int MonthVAR;
    int DayVAR;
    int YearVAR;

    void Swap() {
        DBByteOrderSwapWord(&MonthVAR);
        DBByteOrderSwapWord(&DayVAR);
        DBByteOrderSwapWord(&YearVAR);
    }

public:
    DMLayerHeader() {
        strcpy(DescriptionSTR, "");
        YearVAR = MonthVAR = DayVAR = 0;
    }

    int Read(FILE *file, int swap) {
        if (fread(this, sizeof(DMLayerHeader), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "Reading Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        if (swap) Swap();
        return (DBSuccess);
    }

    int Write(FILE *file) {
        if (fwrite(this, sizeof(DMLayerHeader), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "Writing Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        return (DBSuccess);
    }

    char *Description() { return (DescriptionSTR); }

    void Description(char *str) { strncpy(DescriptionSTR, str, sizeof(DescriptionSTR) - 1); }

    DBInt Month() const { return (MonthVAR); }

    DBInt Day() const { return (DayVAR); }

    DBInt Year() const { return (YearVAR); }
};

class DMDataset : public DMFileHeader {
private:
    char layerNameSTR[DBStringLength], *layerName;
    DMLayerHeader *LayerHeaders;
    int doclength;
    char *documentation;
    void *data;
public:
    void SetDocumentation(void *dataPtr, char *doc) {
        data = dataPtr;
        documentation = doc;
    }

    void SetLayerHeader(DMLayerHeader *header) {
        LayerHeaders = header;
    }

    void SetDocLenght(int docLen) {
        doclength = docLen;
    }

    DBInt Read(FILE *file, DBObjData *data) {
        DBInt i, layer, swap, valueType, valueSize, docLength;
        DMLayerHeader dmLayerHeader;
        DBObjRecord *layerRec, *dataRec;
        DBObjTable *layerTable = data->Table(DBrNLayers);
        DBObjTable *itemTable = data->Table(DBrNItems);
        DBObjTableField *rowNumFLD = layerTable->Field(DBrNRowNum);
        DBObjTableField *colNumFLD = layerTable->Field(DBrNColNum);
        DBObjTableField *cellWidthFLD = layerTable->Field(DBrNCellWidth);
        DBObjTableField *cellHeightFLD = layerTable->Field(DBrNCellHeight);
        DBObjTableField *valueTypeFLD = layerTable->Field(DBrNValueType);
        DBObjTableField *valueSizeFLD = layerTable->Field(DBrNValueSize);
        DBObjTableField *layerFLD = layerTable->Field(DBrNLayer);
        DBGridIF *gridIF;

        if ((swap = DMFileHeader::Read(file)) == DBFault) return (DBFault);
        data->Extent(Extent());
        data->Projection(DBMathGuessProjection(data->Extent()));
        data->Precision(DBMathGuessPrecision(data->Extent()));
        if (FileType() != DMMatrix) {
            CMmsgPrint(CMmsgAppError, "Wrong File Type in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        switch (DataType()) {
            case DMFloat:
                valueType = DBTableFieldFloat;
                valueSize = sizeof(DBFloat4);
                break;
            case DMInt:
                valueType = DBTableFieldInt;
                valueSize = sizeof(DBInt);
                break;
            case DMByte:
                valueType = DBTableFieldInt;
                valueSize = sizeof(DBByte);
                break;
            default:
                CMmsgPrint(CMmsgAppError, "Wrong Data Value Type in: %s %d", __FILE__, __LINE__);
                return (DBFault);
        }
        for (layer = 0; layer < LayerNum(); ++layer) {
            if (dmLayerHeader.Read(file, swap) == DBFault) return (DBFault);
            if (strlen(dmLayerHeader.Description()) > 0) layerName = dmLayerHeader.Description();
            else {
                snprintf(layerNameSTR, sizeof(layerNameSTR), "GHAASLayer%4d", layer + 1);
                layerName = layerNameSTR;
            }
            layerTable->Add(layerName);
            if ((layerRec = layerTable->Item()) == (DBObjRecord *) NULL) return (DBFault);
            rowNumFLD->Int(layerRec, RowNum());
            colNumFLD->Int(layerRec, ColNum());
            cellWidthFLD->Float(layerRec, CellWidth());
            cellHeightFLD->Float(layerRec, CellHeight());
            valueTypeFLD->Int(layerRec, ((DBInt) DBTypeGridDiscrete) == data->Type() ? DBTableFieldInt : valueType);
            valueSizeFLD->Int(layerRec, valueSize);
            dataRec = new DBObjRecord(layerName, ((size_t) ColNum()) * RowNum(), valueSize);
            if (dataRec->Data() == (void *) NULL) {
                delete dataRec;
                return (DBFault);
            }
            (data->Arrays())->Add(dataRec);
            layerFLD->Record(layerRec, dataRec);
        }
        if (fread(&docLength, sizeof(int), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        if (swap) DBByteOrderSwapWord(&docLength);
        if (docLength > 0) {
            char *docString;
            if ((docString = (char *) calloc(docLength, sizeof(char))) == (char *) NULL) {
                CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
            if (fread(docString, docLength, 1, file) != 1) {
                CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
            data->Document(DBDocComment, docString);
            free(docString);
        }
        for (dataRec = (data->Arrays())->First(); dataRec != (DBObjRecord *) NULL; dataRec = (data->Arrays())->Next())
            if (fread(dataRec->Data(), ColNum() * valueSize * RowNum(), 1, file) != 1) {
                CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
        if (swap && valueSize > 1) {
            int i;
            void (*swapFunc)(void *);
            switch (valueSize) {
                case 2:
                    swapFunc = DBByteOrderSwapHalfWord;
                    break;
                case 4:
                    swapFunc = DBByteOrderSwapWord;
                    break;
                case 8:
                    swapFunc = DBByteOrderSwapLongWord;
                    break;
                default:
                    CMmsgPrint(CMmsgAppError, "Wrong Data Value Size in: %s %d", __FILE__, __LINE__);
                    return (DBFault);
            }
            for (dataRec = (data->Arrays())->First();
                 dataRec != (DBObjRecord *) NULL; dataRec = (data->Arrays())->Next())
                for (i = 0; i < ColNum() * RowNum(); ++i) (*swapFunc)((char *) dataRec->Data() + i * valueSize);
        }
        switch (data->Type()) {
            case DBTypeGridDiscrete: {
                DBInt value;
                char nameStr[DBStringLength];
                DBObjRecord *symRec = (data->Table(DBrNSymbols))->Add("Default Symbol");
                DBObjRecord *itemRec;
                DBObjTableField *gridValueFLD = itemTable->Field(DBrNGridValue);
                DBObjTableField *gridSymbolFLD = itemTable->Field(DBrNSymbol);
                DBObjTableField *symbolIDFLD = (data->Table(DBrNSymbols))->Field(DBrNSymbolID);
                DBObjTableField *foregroundFLD = (data->Table(DBrNSymbols))->Field(DBrNForeground);
                DBObjTableField *backgroundFLD = (data->Table(DBrNSymbols))->Field(DBrNBackground);
                DBObjTableField *styleFLD = (data->Table(DBrNSymbols))->Field(DBrNStyle);

                symbolIDFLD->Int(symRec, 0);
                foregroundFLD->Int(symRec, 1);
                backgroundFLD->Int(symRec, 0);
                styleFLD->Int(symRec, 0);
                for (dataRec = (data->Arrays())->First();
                     dataRec != (DBObjRecord *) NULL; dataRec = (data->Arrays())->Next()) {
                    for (i = 0; i < ColNum() * RowNum(); ++i) {
                        switch (valueType) {
                            case DBTableFieldFloat:
                                value = (DBInt) rint(*((float *) ((char *) dataRec->Data() + i * valueSize)));
                                break;
                            case DBTableFieldInt:
                                switch (valueSize) {
                                    case sizeof(DBByte):
                                        value = (DBInt) (*((DBByte *) ((char *) dataRec->Data() + i * valueSize)));
                                        break;
                                    case sizeof(DBShort):
                                        value = (DBInt) (*((DBShort *) ((char *) dataRec->Data() + i * valueSize)));
                                        break;
                                    case sizeof(DBInt):
                                        value = (DBInt) (*((DBInt *) ((char *) dataRec->Data() + i * valueSize)));
                                        break;
                                    default:
                                        CMmsgPrint(CMmsgAppError, "Wrong Data Size in: %s %d", __FILE__, __LINE__);
                                        return (DBFault);
                                }
                                break;
                            default:
                                CMmsgPrint(CMmsgAppError, "Wrong Data Type in: %s %d", __FILE__, __LINE__);
                                return (DBFault);
                        }
                        snprintf(nameStr, sizeof(nameStr), "Category%04d", value);
                        if ((itemRec = itemTable->Item(nameStr)) == (DBObjRecord *) NULL) {
                            if ((itemRec = itemTable->Add(nameStr)) == (DBObjRecord *) NULL) {
                                CMmsgPrint(CMmsgAppError, "Item Object Creation Error in: %s %d", __FILE__, __LINE__);
                                return (DBFault);
                            }
                            gridValueFLD->Int(itemRec, value);
                            gridSymbolFLD->Record(itemRec, symRec);
                        }
                        value = itemRec->RowID();
                        switch (valueSize) {
                            case sizeof(DBByte):
                                *((DBByte *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            case sizeof(DBShort):
                                *((DBShort *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            case sizeof(DBInt):
                                *((DBInt *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            default:
                                CMmsgPrint(CMmsgAppError, "Wrong Data Size in: %s %d", __FILE__, __LINE__);
                                return (DBFault);
                        }
                    }
                }
                itemTable->ListSort(gridValueFLD);
                for (dataRec = (data->Arrays())->First();
                     dataRec != (DBObjRecord *) NULL; dataRec = (data->Arrays())->Next()) {
                    for (i = 0; i < ColNum() * RowNum(); ++i) {
                        switch (valueSize) {
                            case sizeof(DBByte):
                                value = (DBInt) (*((DBByte *) ((char *) dataRec->Data() + i * valueSize)));
                                break;
                            case sizeof(DBShort):
                                value = (DBInt) (*((DBShort *) ((char *) dataRec->Data() + i * valueSize)));
                                break;
                            case sizeof(DBInt):
                                value = (DBInt) (*((DBInt *) ((char *) dataRec->Data() + i * valueSize)));
                                break;
                            default:
                                CMmsgPrint(CMmsgAppError, "Wrong Data Size in: %s %d", __FILE__, __LINE__);
                                return (DBFault);
                        }
                        itemRec = itemTable->Item(value);
                        value = itemRec->ListPos();
                        switch (valueSize) {
                            case sizeof(DBByte):
                                *((DBByte *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            case sizeof(DBShort):
                                *((DBShort *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            case sizeof(DBInt):
                                *((DBInt *) ((char *) dataRec->Data() + i * valueSize)) = value;
                                break;
                            default:
                                CMmsgPrint(CMmsgAppError, "Wrong Data Size in: %s %d", __FILE__, __LINE__);
                                return (DBFault);
                        }
                    }
                }
                itemTable->ItemSort();
                gridIF = new DBGridIF(data);
                gridIF->DiscreteStats();
                delete gridIF;
            }
                break;
            case DBTypeGridContinuous: {
                DBObjTableField *missingValueFLD = itemTable->Field(DBrNMissingValue);

                for (dataRec = (data->Arrays())->First();
                     dataRec != (DBObjRecord *) NULL; dataRec = (data->Arrays())->Next()) {
                    itemTable->Add(dataRec->Name());
                    missingValueFLD->Float(itemTable->Item(), MissingValue());
                }
                gridIF = new DBGridIF(data);
                gridIF->RecalcStats();
                delete gridIF;
                data->Flags(DBDataFlagDispModeContBlueRed, DBSet);
                break;
            }
            default:
                break;
        }
        return (DBSuccess);
    }

    DBInt Write(FILE *file, DBObjData *data) {
        char *dmRecord;
        DBInt layerID, docLen, dbType, intVal;
        DBFloat floatVal;
        DBPosition pos;
        DBObjRecord *layerRec;
        DBGridIF *gridIF = new DBGridIF(data);
        DMLayerHeader dmLayerHeader;

        dbType = gridIF->ValueType();
        switch (dbType) {
            case DBTableFieldFloat:
                DataType(DMFloat);
                break;
            case DBTableFieldInt:
                DataType(gridIF->ValueSize() > 1 ? DMInt : DMByte);
                break;
        }
        CellWidth(gridIF->CellWidth());
        CellHeight(gridIF->CellHeight());
        Extent(data->Extent());
        LayerNum(gridIF->LayerNum());
        RowNum(gridIF->RowNum());
        ColNum(gridIF->ColNum());
        dmRecord = (char *) calloc(RowNum() * ColNum(), DataType() != DMByte ? sizeof(int) : sizeof(char));
        if (dmRecord == (char *) NULL) {
            CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }

        DMFileHeader::Write(file);
        for (layerID = 0; layerID < gridIF->LayerNum(); ++layerID) {
            layerRec = gridIF->Layer(layerID);
            dmLayerHeader.Description(layerRec->Name());
            dmLayerHeader.Write(file);
        }
        docLen = strlen(data->Document(DBDocComment));
        if (fwrite(&docLen, sizeof(int), 1, file) != 1) {
            CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
            return (DBFault);
        }
        if (docLen > 0) {
            if (fwrite(data->Document(DBDocComment), docLen, 1, file) != 1) {
                CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
        }
        for (layerID = 0; layerID < gridIF->LayerNum(); ++layerID) {
            layerRec = gridIF->Layer(layerID);
            switch (data->Type()) {
                case DBTypeGridContinuous:
                    layerRec = gridIF->Layer(layerID);
                    if (dbType == DBTableFieldFloat) {
                        for (pos.Row = 0; pos.Row < RowNum(); pos.Row++)
                            for (pos.Col = 0; pos.Col < ColNum(); pos.Col++)
                                if (gridIF->Value(layerRec, pos, &floatVal))
                                    ((float *) dmRecord)[(RowNum() - pos.Row - 1) * ColNum() +
                                                         pos.Col] = (float) floatVal;
                                else
                                    ((float *) dmRecord)[(RowNum() - pos.Row - 1) * ColNum() +
                                                         pos.Col] = MissingValue();
                    }
                    else {
                        for (pos.Row = 0; pos.Row < RowNum(); pos.Row++)
                            for (pos.Col = 0; pos.Col < ColNum(); pos.Col++)
                                if (gridIF->Value(layerRec, pos, &intVal)) {
                                    if (DataType() == DMInt)
                                        ((int *) dmRecord)[(RowNum() - pos.Row - 1) * ColNum() +
                                                           pos.Col] = (int) intVal;
                                    else
                                        dmRecord[(RowNum() - pos.Row - 1) * ColNum() + pos.Col] = intVal;
                                }
                                else {
                                    if (DataType() == DMInt)
                                        ((int *) dmRecord)[(RowNum() - pos.Row - 1) * ColNum() +
                                                           pos.Col] = (int) MissingValue();
                                    else
                                        dmRecord[(RowNum() - pos.Row - 1) * ColNum() + pos.Col] = -99;
                                }

                    }
                    break;
                case DBTypeGridDiscrete:
                    for (pos.Row = 0; pos.Row < RowNum(); pos.Row++)
                        for (pos.Col = 0; pos.Col < ColNum(); pos.Col++) {
                            intVal = gridIF->GridValue(layerRec, pos);
                            if (DataType() == DMInt)
                                ((int *) dmRecord)[(RowNum() - pos.Row - 1) * ColNum() + pos.Col] = intVal;
                            else
                                dmRecord[(RowNum() - pos.Row - 1) * ColNum() + pos.Col] = -99;

                        }
                    break;
                default:
                    CMmsgPrint(CMmsgAppError, "Invalid Data Type in: %s %d", __FILE__, __LINE__);
                    free(dmRecord);
                    delete gridIF;
                    return (DBFault);
            }
            if ((DBInt) fwrite(dmRecord, DataType() == DMByte ? sizeof(char) : sizeof(int), DataPointNum(), file) !=
                DataPointNum()) {
                CMmsgPrint(CMmsgSysError, "File Writing Error in: %s %d", __FILE__, __LINE__);
                return (DBFault);
            }
        }
        free(dmRecord);
        delete gridIF;
        return (DBSuccess);
    }
};


int DBImportDMGrid(DBObjData *data, FILE *file) {
    DMDataset dmDataset;
    return (dmDataset.Read(file, data));
}

int DBImportDMGrid(DBObjData *data, const char *fileName) {
    FILE *file;
    DBInt ret;

    if ((file = fopen(fileName, "r")) == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "File Opening Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    ret = DBImportDMGrid(data, file);
    fclose(file);
    return (ret);
}

int DBExportDMGrid(DBObjData *data, FILE *file) {
    DMDataset dmDataset;
    return (dmDataset.Write(file, data));
}

int DBExportDMGrid(DBObjData *data, const char *fileName) {
    FILE *file;
    DBInt ret;

    if ((file = fopen(fileName, "w")) == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "File Opening Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }

    ret = DBExportDMGrid(data, file);

    fclose(file);
    if (ret == DBFault) unlink(fileName);
    return (ret);
}
