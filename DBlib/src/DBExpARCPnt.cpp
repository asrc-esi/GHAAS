/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

DBExpARCPnt.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>

int DBExportARCGenPoint(DBObjData *data, char *fileName) {
    DBInt ret;
    FILE *file;

    if ((file = fopen(fileName, "w")) == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "ARC Generate File Opening Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    ret = DBExportARCGenPoint(data,file);
    fclose (file);
    return (ret);
}

int DBExportARCGenPoint(DBObjData *data, FILE *file) {
    DBCoordinate coord;
    DBObjTable *items = data->Table(DBrNItems);
    DBObjTableField *coordField = items->Field(DBrNCoord);
    DBObjRecord *pntRec;

    for (pntRec = items->First(); pntRec != (DBObjRecord *) NULL; pntRec = items->Next()) {
        coord = coordField->Coordinate(pntRec);
        fprintf(file, "%d,%f,%f\n", pntRec->RowID() + 1, coord.X, coord.Y);
    }
    fprintf(file, "END\n");
    return (DBSuccess);
}
