/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDgrdImport.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <DB.hpp>
#include <DBif.hpp>

using namespace std;
/*=========================================================================*/
#define RGISLaoutByRow            0
#define RGISLaoutByCol            1

#define RGISItemOrderTopDown    0
#define RGISItemOrderBottomUp    1

#define RGISGridBinary            0
#define RGISGridASCII            1

#define RGISBinTypeByte            0
#define RGISBinTypeShort        1
#define RGISBinTypeLong            2
#define RGISBinTypeSingle        3
#define RGISBinTypeDouble        4

/*==========================================================================*/

int colNum, rowNum, tempInt;
char *text, listFileName[DBDataFileNameLen], buffer[256];
char outFile[DBDataFileNameLen];
float cellWidth, cellHeight, llXCoord, llYCoord;
static int layout = 0, rowOrder = 0, fileType = 0, binaryType = 0, byteOrder = 0;
static int llCellCol, llCellRow = 0, skipHeader = 0, skipPad = 0, listFile = false;
static float missingVal = DBDefaultMissingFloatVal;
float temp;
bool Batch_Mode = false;
bool Continuous_Grid_Mode = false;

int readHeader(bool ListFileMode);

void skipComments() {
    while (cin.peek() == '#' || cin.peek() == '\t' || cin.peek() == ' ')
        cin.ignore(1000000, '\n'); //skip the whole line!
}

int Exists_In_Arguments(char *argShort, char *argLong, int argc, char *argv[]) {
    int i = 1;
    while (i < argc) {
        if ((strcmp(argShort, argv[i]) == 0) || (strcmp(argLong, argv[i]) == 0)) return i + 1;
        i++;
    }
    return -1;
}


int Open_File(int argc, char *argv[]) {
    int i = 1;
    char *test;
    while (i < argc) {
        test = argv[i];
        if (*test != '-') {
            if (access(argv[i], R_OK) == 0) {
                if (fopen(argv[i], "r") == (FILE *) NULL) {
                    CMmsgPrint(CMmsgUsrError, "File opening error: %s!", argv[i]);
                    return (0);
                }
                else
                    strcpy(listFileName, argv[i]);
                return 1;
            }
        }
        i++;
    }
    printf("no input file name was found.. Im going to read in from stdin!.\n");
    return 0;
}


int getValuesStandardIn(bool batchValue) {

    if (batchValue == false) cout << "\tFile Name?> ";
    skipComments();
    cin >> listFileName;

    if (batchValue == false) cout << "\tNumber Of Columns?> ";
    skipComments();
    cin >> colNum;

    if (batchValue == false) cout << "\tNumber Of Rows?> ";
    skipComments();
    cin >> rowNum;
    if (colNum <= 0 || rowNum <= 0) {
        cout << "Err-- the column/row numbers must be greater than 0!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tGrid Layout: 0=By Row, 1=By Column?> ";
    skipComments();
    cin >> layout;
    if (layout < 0 || layout > 1) {
        cout << "Err-- layout number is not a boolean!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tRow Order: 0=Top Down, 1=Bottom Up?> ";
    skipComments();
    cin >> rowOrder;
    if (rowOrder < 0 || rowOrder > 1) {
        cout << "Err-- row order number is not a boolean!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tFile Type: 0=Binary, 1=Ascii?> ";
    skipComments();
    cin >> fileType;
    if (fileType < 0 || fileType > 1) {
        cout << "File type number is not a boolean!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tBinary Type: 0=byte, 1=short, 2=long, 3=float, 4=double?> ";
    skipComments();
    cin >> binaryType;
    if (binaryType < 0 || binaryType > 4) {
        cout << "ERR- Binary_Type: invalid numbers!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tByte Order: 0=Big Endian, 1=Little Endian (ZERO if the file is ascii!)?> ";
    skipComments();
    cin >> byteOrder;
    if (byteOrder < 0 || byteOrder > 1) {
        cout << "ERR- Byte Order: invalid numbers!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tLLCell_(Column)?> ";
    skipComments();
    cin >> llCellCol;

    if (batchValue == false) cout << "\tLLCell_(Row)?> ";
    skipComments();
    cin >> llCellRow;

    if (batchValue == false) cout << "\tCell size? (X)> ";
    skipComments();
    cin >> cellWidth;
    if (cellWidth <= 0) {
        cout << "ERR- cell width: Invalid number!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tCell size? (Y)> ";
    skipComments();
    cin >> cellHeight;
    if (cellHeight < 0) {
        cout << "ERR- cell height: Invalid number!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tLLCorner? (X)> ";
    skipComments();
    cin >> llXCoord;

    if (batchValue == false) cout << "\tLLCorner? (Y)> ";
    skipComments();
    cin >> llYCoord;

    if (batchValue == false) cout << "\tMissing Data Value?> ";
    skipComments();
    cin >> missingVal;

    if (batchValue == false) cout << "\tHeader?> ";
    skipComments();
    cin >> skipHeader;
    if (skipHeader < 0) {
        cout << "Err-- invalid value for header field\n";
        return 0;
    }

    if (batchValue == false) cout << "\tList File? (0=no, 1=yes)> ";
    skipComments();
    cin >> listFile;
    if (listFile > 1 || listFile < 0) {
        cout << "ERR- listfile: Invalid number!\n";
        return 0;
    }

    if (batchValue == false) cout << "\tOuptut file?>";
    skipComments();
    cin >> outFile;

    if (batchValue == false) cout << "\tGrid Type: 0=discrete, 1=continuous?> ";
    skipComments();
    cin >> tempInt;
    if (tempInt < 0 || tempInt > 1) {
        cout << "Err-- Grid type field has invalid value!\n";
        return 0;
    }
    if (tempInt == 0) Continuous_Grid_Mode = false; else Continuous_Grid_Mode = true;

    return 1;
}

int getValuesFromFileHeader(bool batchMode) {

    bool ListFileMode = false;

    fileType = 1; //set the file type to ascii
    if (batchMode == true) skipComments();

    if (batchMode == false) cout << "\tBinary Type: 0=byte, 1=short, 2=long, 3=float, 4=double?> ";
    skipComments();
    cin >> binaryType;
    if (binaryType < 0 || binaryType > 4) {
        cout << "ERR- Binary_Type: invalid numbers!\n";
        return 0;
    }

    if (batchMode == false) cout << "\tMissing Data Value?> ";
    skipComments();
    cin >> missingVal;

    if (batchMode == false) cout << "\tList File? (0=no, 1=yes)> ";
    skipComments();
    cin >> listFile;
    if (listFile > 1 || listFile < 0) {
        cout << "ERR- listfile: Invalid number!\n";
        return 0;
    }
    if (listFile == 1) ListFileMode = true;

    if (batchMode == false) cout << "\tOuptut file?>";
    skipComments();
    cin >> outFile;

    if (batchMode == false) cout << "\tGrid Type: 0=discrete, 1=continuous?> ";
    skipComments();
    cin >> tempInt;
    if (tempInt < 0 || tempInt > 1) {
        cout << "Err-- Grid type field has invalid value!\n";
        return 0;
    }
    if (tempInt == 0) Continuous_Grid_Mode = false; else Continuous_Grid_Mode = true;

    //I assume these to be defaults for all ascii files
    llCellCol = 0;
    llCellRow = 0;
    skipHeader = 6;

    if (!readHeader(ListFileMode)) return 0;
    else return 1;
}

int readHeader(bool ListFileMode) {
    char JUNK[40];
    char testFileName[DBDataFileNameLen];

    if (ListFileMode == true) {
        ifstream textFile(listFileName);
        textFile >> testFileName;
    }
    else strcpy(testFileName, listFileName);

    ifstream infile(testFileName);
    bool cellCenter = false;

    while (infile.good()) {
        infile >> JUNK;
        infile >> colNum;                   //number of columns
        infile >> JUNK;
        infile >> rowNum;
        infile >> JUNK;   //number of rows
        if (strcmp(JUNK, "XLLCENTER") == 0) cellCenter = true;   //is this xllcorner or xllcenter?
        infile >> llXCoord;                                    //llxcoord
        infile >> JUNK;
        infile >> llYCoord;                 //llycoord
        infile >> JUNK;
        infile >> cellWidth;
        cellHeight = cellWidth; //cellsize
        infile >> JUNK;
        infile >> missingVal;               //nodata_val

        if (cellCenter) {
            //if we are measuring from the center of a cell instead of the corner
            //set it so it reads from the corner.
            llXCoord = llXCoord - (cellWidth / 2);
            llYCoord = llYCoord - (cellWidth / 2);
        }

        return 1;
    }

    cout << "I couldn't read the file: " << listFileName << "... check the file for errors!\n";
    if (ListFileMode)
        cout << "List file: the problem could either be in there or in " << testFileName << "\n";
    return 0;
}

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s - imports data files into ghdb format", arg0);
    CMmsgPrint(CMmsgInfo, "  options:");
    CMmsgPrint(CMmsgInfo, "    -b,--batch (optional) turns the prompt for information off");
    CMmsgPrint(CMmsgInfo, "    -h,--help (optional) shows usage");
    CMmsgPrint(CMmsgInfo, "    <filename> (optional) grdimport will attempt to automatically");
    CMmsgPrint(CMmsgInfo, "    read the standard header from an ascii file.");
    CMmsgPrint(CMmsgInfo, "  examples:");
    CMmsgPrint(CMmsgInfo, "    grdImport.Linux -b < completeInputFile.txt");
    CMmsgPrint(CMmsgInfo, "      Don't prompt for information, just read completeFile.txt for arguments");
    CMmsgPrint(CMmsgInfo, "    grdImport.Linux -b someFile.asc < partialInputFile.txt");
    CMmsgPrint(CMmsgInfo, "      Don't prompt for information, just read the header of the ascii file and");
    CMmsgPrint(CMmsgInfo, "      partialInputFile.txt for all of the arguments.");
    CMmsgPrint(CMmsgInfo, "    grdImport.Linux <--prompt for all information step by step");
    CMmsgPrint(CMmsgInfo, "      grdImport.Linux someFile.asc <--get as much info as you can out of");
    CMmsgPrint(CMmsgInfo, "      the header, and prompt for whatever else is needed.");
}

int main(int argc, char *argv[]) {
    if (Exists_In_Arguments((char *) "-h", (char *) "--help", argc, argv) != -1) {
        _CMDprintUsage(argv[0]);
        return 0;
    }
    if (Exists_In_Arguments((char *) "-b", (char *) "--batch", argc, argv) != -1) Batch_Mode = true;

    if (Open_File(argc, argv) == 0) {
        if (!getValuesStandardIn(Batch_Mode)) return 0;
    }
    else {
        //we have the file name, and we can get all of the values we need
        //from opening the file
        if (!getValuesFromFileHeader(Batch_Mode)) return 0;
    }

    DBObjData *grdData;

    if (Continuous_Grid_Mode) grdData = new DBObjData("", DBTypeGridContinuous);
    else                      grdData = new DBObjData("", DBTypeGridDiscrete);

    grdData->Name("Untitled");
    FILE *inFILE, *lstFILE = (FILE *) NULL;
    char fileName[DBDataFileNameLen+256], recordName[DBStringLength]; //TODO: ugly fixed size array from Pete.
    DBAddress item;
    DBInt pathLen, itemSize, chunk, i, j, row, col, recordLen;
    DBCoordinate coord;
    DBRegion extent;
    DBObjTable *layerTable = grdData->Table(DBrNLayers);
    DBObjTable *itemTable = grdData->Table(DBrNItems);
    DBObjTableField *missingValueFLD = grdData->Type() == DBTypeGridContinuous ? itemTable->Field(DBrNMissingValue) : (DBObjTableField *) NULL;
    DBObjTableField *rowNumFLD = layerTable->Field(DBrNRowNum);
    DBObjTableField *colNumFLD = layerTable->Field(DBrNColNum);
    DBObjTableField *cellWidthFLD = layerTable->Field(DBrNCellWidth);
    DBObjTableField *cellHeightFLD = layerTable->Field(DBrNCellHeight);
    DBObjTableField *valueTypeFLD = layerTable->Field(DBrNValueType);
    DBObjTableField *valueSizeFLD = layerTable->Field(DBrNValueSize);
    DBObjTableField *layerFLD = layerTable->Field(DBrNLayer);
    DBObjRecord *layerRec, *itemRec, *dataRec;
    DBGridIF *gridIF;

    coord.X = llXCoord;
    coord.Y = llYCoord;
    extent.Expand(coord);
    coord.X += colNum * cellWidth;
    coord.Y += rowNum * cellHeight;
    extent.Expand(coord);
    grdData->Extent(extent);
    grdData->Projection(DBMathGuessProjection(extent));
    grdData->Precision(DBMathGuessPrecision(extent));

    switch (binaryType) {
        case RGISBinTypeByte:
            itemSize = sizeof(DBByte);
            break;
        case RGISBinTypeShort:
            itemSize = sizeof(DBShort);
            break;
        case RGISBinTypeLong:
            itemSize = sizeof(DBInt);
            break;
        case RGISBinTypeSingle:
            itemSize = sizeof(DBFloat4);
            break;
        case RGISBinTypeDouble:
            itemSize = sizeof(DBFloat);
            break;
        default:
            CMmsgPrint(CMmsgAppError, "Invalid Data Type in: %s %d", __FILE__, __LINE__);
            delete grdData;
            return 0;
    }

    for (pathLen = strlen(listFileName) - 1; (pathLen > 0) && (listFileName[pathLen] != '/'); --pathLen);
    if (listFileName[pathLen] == '/') ++pathLen;
    if (listFile) {
        if ((lstFILE = fopen(listFileName, "r")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgSysError, "List File Openining Error in: %s %d", __FILE__, __LINE__);
            delete grdData;
            return 0;
        }
        listFileName[pathLen] = '\0';
    }

    while (true) {
        if (listFile) {
            if (fgets(buffer, sizeof(buffer) - 2, lstFILE) != buffer) break;
            if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
            if (buffer[0] == '/') strncpy(fileName, buffer, sizeof(fileName) - 1);
            else
                snprintf(fileName, sizeof(fileName), "%s%s", listFileName, buffer);
            for (i = strlen(fileName) - 1; (i > 0) && fileName[i] != '/'; --i);
            if (fileName[i] == '/') ++i;
            strncpy(recordName, fileName + i, sizeof(recordName) - 1);
        }
        else {
            if (layerTable->ItemNum() > 0) break;
            strncpy(fileName, listFileName, sizeof(fileName) - 1);
            strncpy(recordName, listFileName + pathLen, sizeof(recordName) - 1);
        }

        if ((inFILE = fopen(fileName, "r")) == (FILE *) NULL) {
            CMmsgPrint(CMmsgSysError, "File Openining Error in: %s %d", __FILE__, __LINE__);
            delete grdData;
            return 0;
        }
        if (fileType == RGISGridBinary) {
            int skip = skipHeader;
            while (skip > 0) {
                chunk = skip < (int) sizeof(buffer) ? skip : (int) sizeof(buffer);
                if (fread(buffer, chunk, 1, inFILE) != 1) {
                    CMmsgPrint(CMmsgSysError, "File Reading Error in: %s %d", __FILE__, __LINE__);
                    fclose(inFILE);
                    delete grdData;
                }
                skip -= chunk;
            }
        }
        else
            for (chunk = 0; chunk < skipHeader; ++chunk)
                do fgets(buffer, sizeof(buffer) - 2, inFILE);
                while (buffer[strlen(buffer) - 1] != '\n');

        for (i = 0; i < (int) strlen(recordName); ++i) if (recordName[i] == '.') recordName[i] = ' ';
        layerTable->Add(recordName);
        if ((layerRec = layerTable->Item()) == (DBObjRecord *) NULL) {
            fclose(inFILE);
            delete grdData;
            return 0;
        }
        rowNumFLD->Int(layerRec, rowNum);
        colNumFLD->Int(layerRec, colNum);
        cellWidthFLD->Float(layerRec, cellWidth);
        cellHeightFLD->Float(layerRec, cellHeight);

        switch (binaryType) {
            case RGISBinTypeByte:
            case RGISBinTypeShort:
            case RGISBinTypeLong:
                valueTypeFLD->Int(layerRec, DBTableFieldInt);
                break;
            case RGISBinTypeSingle:
            case RGISBinTypeDouble:
                valueTypeFLD->Int(layerRec, DBTableFieldFloat);
                break;
            default:
                CMmsgPrint(CMmsgAppError, "Invalid Data Type in: %s %d", __FILE__, __LINE__);
                fclose(inFILE);
                delete grdData;
                return 0;
        }
        valueSizeFLD->Int(layerRec, itemSize);
        dataRec = new DBObjRecord(layerRec->Name(), (DBAddress) colNum * (DBAddress) rowNum, valueSizeFLD->Int(layerRec));
        if (dataRec->Data() == (void *) NULL) {
            delete dataRec;
            delete grdData;
            fclose(inFILE);
            return 0;
        }
        (grdData->Arrays())->Add(dataRec);
        layerFLD->Record(layerRec, dataRec);
        if (grdData->Type() == DBTypeGridContinuous) {
            itemTable->Add(layerRec->Name());
            if ((itemRec = itemTable->Item()) == (DBObjRecord *) NULL) {
                fclose(inFILE);
                delete grdData;
                return 0;
            }
            missingValueFLD->Float(itemRec,  (DBFloat) missingVal);
        }

        if (fileType == RGISGridBinary) {
            recordLen = (layout == RGISLaoutByRow ? colNum : rowNum) * itemSize + skipPad;
            for (j = 0; (chunk = fread(buffer, 1, sizeof(buffer), inFILE)) > 0; ++j)
                for (item = 0; item < chunk; item += itemSize) {//this is the itemsize declaration
                    //it is dependent upon the
                    if (byteOrder != DBByteOrder())
                        switch (itemSize) {
                            default:
                                break;
                            case 2:
                                DBByteOrderSwapHalfWord(buffer + item);
                                break;
                            case 4:
                                DBByteOrderSwapWord(buffer + item);
                                break;
                            case 8:
                                DBByteOrderSwapLongWord(buffer + item);
                                break;
                        }
                    if (layout == RGISLaoutByRow) {
                        if ((row = (DBInt)  ((DBAddress) j * sizeof(buffer) + item) / recordLen) >= rowNum) continue;
                        if ((col = (DBInt) (((DBAddress) j * sizeof(buffer) + item) % recordLen) / itemSize) >= colNum) continue;
                    }
                    else {
                        if ((col = (DBInt)  ((DBAddress) j * sizeof(buffer) + item) / recordLen) >= colNum) continue;
                        if ((row = (DBInt) (((DBAddress) j * sizeof(buffer) + item) % recordLen) / itemSize) >= rowNum) continue;
                    }
                    if (rowOrder == RGISItemOrderBottomUp) row = rowNum - row - 1;
                    col -= llCellCol;
                    col = col < 0 ? colNum + col : col;
                    row -= llCellRow;
                    row = row < 0 ? rowNum + row : row;
                    memcpy(((char *) dataRec->Data()) +
                           ((DBAddress) row * (DBAddress) colNum + (DBAddress) col) * (DBAddress) itemSize,
                           buffer + (DBAddress) i, itemSize);
                }
        }
        else {
            DBFloat val;
            i = j = 0;
            while (fscanf(inFILE, "%lf", &val) == 1) {
                if (j >= rowNum) continue;
                if (i >= colNum) continue;
                switch (binaryType) {
                    case RGISBinTypeByte:
                        ((DBByte *) buffer)[0] = (DBByte) val;
                        break;
                    case RGISBinTypeShort:
                        ((DBShort *) buffer)[0] = (DBShort) val;
                        break;
                    case RGISBinTypeLong:
                        ((DBInt *) buffer)[0] = (DBInt) val;
                        break;
                    case RGISBinTypeSingle:
                        ((DBFloat4 *) buffer)[0] = (DBFloat4) val;
                        break;
                    case RGISBinTypeDouble:
                        ((DBFloat *) buffer)[0] = (DBFloat) val;
                        break;
                }
                col = i;
                row = j;
                if (rowOrder == RGISItemOrderBottomUp) row = rowNum - row - 1;
                col -= llCellCol;
                col = col < 0 ? colNum + col : col;
                row -= llCellRow;
                row = row < 0 ? rowNum + row : row;
                memcpy(((char *) dataRec->Data()) +
                       ((DBAddress) row * (DBAddress) colNum + (DBAddress) col) * (DBAddress) itemSize, buffer, itemSize);
                if (layout == RGISLaoutByRow) {
                    i += 1;
                    if (i >= colNum) {
                        i = 0;
                        j += 1;
                    }
                }
                else {
                    j += 1;
                    if (j >= rowNum) {
                        j = 0;
                        i += 1;
                    }
                }
            }
        }
        fclose(inFILE);
    }
    if (listFile) fclose(lstFILE);
    gridIF = new DBGridIF(grdData);
    if (grdData->Type() == DBTypeGridContinuous) gridIF->RecalcStats();
    else {
        DBInt intVal;
        DBObjRecord *symRec = (grdData->Table(DBrNSymbols))->Add("Default Symbol");
        DBObjTableField *gridValueFLD = itemTable->Field(DBrNGridValue);
        DBObjTableField *gridSymbolFLD = itemTable->Field(DBrNSymbol);
        DBObjTableField *symbolIDFLD = (grdData->Table(DBrNSymbols))->Field(DBrNSymbolID);
        DBObjTableField *foregroundFLD = (grdData->Table(DBrNSymbols))->Field(DBrNForeground);
        DBObjTableField *backgroundFLD = (grdData->Table(DBrNSymbols))->Field(DBrNBackground);
        DBObjTableField *styleFLD = (grdData->Table(DBrNSymbols))->Field(DBrNStyle);

        symbolIDFLD->Int(symRec, 0);
        foregroundFLD->Int(symRec, 1);
        backgroundFLD->Int(symRec, 0);
        styleFLD->Int(symRec, 0);
        for (dataRec = (grdData->Arrays())->First(); dataRec != (DBObjRecord *) NULL; dataRec = (grdData->Arrays())->Next()) {
            for (item = 0; item < colNum * rowNum; ++item) {
                switch (itemSize) {
                    case 1:
                        intVal = (DBInt) (*((DBByte *) ((char *) dataRec->Data() + item * itemSize)));
                        break;
                    case 2:
                        intVal = (DBInt) (*((DBShort *) ((char *) dataRec->Data() + item * itemSize)));
                        break;
                    default:
                        intVal = (DBInt) (*((DBInt *) ((char *) dataRec->Data() + item * itemSize)));
                        break;
                }
                if (intVal != (DBInt) missingVal) {
                    snprintf(buffer, sizeof(buffer), "Category%010d", intVal);
                    if ((itemRec = itemTable->Item(buffer)) == (DBObjRecord *) NULL) {
                        if ((itemRec = itemTable->Add(buffer)) == (DBObjRecord *) NULL) {
                            CMmsgPrint(CMmsgAppError, "Item Object Creation Error in: %s %d", __FILE__, __LINE__);
                            delete gridIF;
                            delete grdData;
                            return 0;
                        }
                        gridValueFLD->Int(itemRec, intVal);
                        gridSymbolFLD->Record(itemRec, symRec);
                    }
                    intVal = itemRec->RowID();
                    switch (itemSize) {
                        case 1:  *((DBByte *)  ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                        case 2:  *((DBShort *) ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                        default: *((DBInt *)   ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                    }
                }
                else {
                    switch (itemSize) {
                        case 1:  *((DBByte *)  ((char *) dataRec->Data() + item * itemSize)) = DBFault; break;
                        case 2:  *((DBShort *) ((char *) dataRec->Data() + item * itemSize)) = DBFault; break;
                        default: *((DBInt *)   ((char *) dataRec->Data() + item * itemSize)) = DBFault; break;
                    }
                }
            }
        }
        itemTable->ListSort(gridValueFLD);
        for (dataRec = (grdData->Arrays())->First();
             dataRec != (DBObjRecord *) NULL; dataRec = (grdData->Arrays())->Next()) {
            for (item = 0; item < colNum * rowNum; ++item) {
                switch(itemSize) {
                    case 1:  intVal = (DBInt) (*((DBByte *)  ((char *) dataRec->Data() + item * itemSize))); break;
                    case 2:  intVal = (DBInt) (*((DBShort *) ((char *) dataRec->Data() + item * itemSize))); break;
                    default: intVal = (DBInt) (*((DBInt *)   ((char *) dataRec->Data() + item * itemSize))); break;
                }
                if (intVal != DBFault) {
                    itemRec = itemTable->Item(intVal);
                    intVal = itemRec->ListPos();
                }
                switch (itemSize) {
                    case 1:   *((DBByte *)  ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                    case 2:   *((DBShort *) ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                    default:  *((DBInt *)   ((char *) dataRec->Data() + item * itemSize)) = intVal; break;
                }
            }
        }
        itemTable->ItemSort();
        gridIF->DiscreteStats();
    }
    delete gridIF;
    if (grdData->Write(outFile) == DBFault) return (-1);
    return 0;
}
