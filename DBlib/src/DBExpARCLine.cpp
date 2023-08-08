/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

DBExpARCPnt.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <DBif.hpp>

int DBExportARCGenLine(DBObjData *data, char *fileName) {
    DBInt ret;
    FILE *file;

    if ((file = fopen(fileName, "w")) == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "ARC Generate File Opening Error in: %s %d", __FILE__, __LINE__);
        return (DBFault);
    }
    ret = DBExportARCGenLine (data, file);
    fclose (file);
    return (ret);
}

int DBExportARCGenLine (DBObjData *data, FILE *file) {
    DBInt vertex, vertexNum;
    DBCoordinate *coords, nodeCoord;
    DBObjTable *items = data->Table(DBrNItems);
    DBObjRecord *lineRec;
    DBVLineIF *lineIF = new DBVLineIF (data);

    for (lineRec = items->First(); lineRec != (DBObjRecord *) NULL; lineRec = items->Next()) {
        fprintf (file,"%d\n",lineRec->RowID () + 1);
        nodeCoord = lineIF->FromCoord(lineRec);
        fprintf(file, "%f,%f\n", nodeCoord.X, nodeCoord.Y);
        vertexNum = lineIF->VertexNum(lineRec);
        if (vertexNum > 0) {
            coords = lineIF->Vertexes(lineRec);
            for (vertex = 0;vertex < vertexNum; ++vertex)
                fprintf(file, "%f,%f\n", coords [vertex].X, coords [vertex].Y);
        }
        nodeCoord = lineIF->ToCoord(lineRec);
        fprintf(file, "%f,%f\n", nodeCoord.X, nodeCoord.Y);
        fprintf(file, "END\n");
    }
    fprintf(file, "END\n");
    return (DBSuccess);
}
