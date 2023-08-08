/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - CCNY

DBExpShapefile.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <DBif.hpp>
#include <shapefil.h>

int DBExportShapefile (DBObjData *data, const char *fileName) {
    DBInt ret = DBFault;
    SHPHandle shpHandle;

    switch(data->Type ()) {
        case DBTypeVectorPoint:
            if ((shpHandle = SHPCreate(fileName, SHPT_POINT)) == (SHPHandle) NULL)
                CMmsgPrint (CMmsgUsrError,"Invalid shapefile");
            else {
                DBInt objID;
                DBCoordinate coord;
                DBObjTable *items = data->Table(DBrNItems);
                DBObjTableField *coordField = items->Field(DBrNCoord);
                DBObjRecord *pntRec;
                double padfX, padfY;
                SHPObject *shpObject;

                for (pntRec = items->First(); pntRec != (DBObjRecord *) NULL; pntRec = items->Next()) {
                    coord = coordField->Coordinate(pntRec);
                    padfX = coord.X;
                    padfY = coord.Y;
                    if ((shpObject = SHPCreateSimpleObject (SHPT_POINT,1, &padfX, &padfY, (double *) NULL)) == (SHPObject *) NULL) {
                        CMmsgPrint (CMmsgUsrError,"SHPObject creation error");
                        break;
                    }
                    else {
                        if ((objID = SHPWriteObject (shpHandle,-1,shpObject)) < 0) {
                            CMmsgPrint (CMmsgSysError, "SHPObject writing error: %d", objID);
                            break;
                        }
                    }
                }
                SHPClose (shpHandle);
                ret = pntRec == (DBObjRecord *) NULL ? DBSuccess : DBFault;
            }
            break;
        case DBTypeVectorLine:
            if ((shpHandle = SHPCreate(fileName, SHPT_ARC)) == (SHPHandle) NULL)
                CMmsgPrint (CMmsgUsrError,"Invalid shapefile");
            else {
                DBInt vertex, vertexNum;
                DBCoordinate *coords, nodeCoord;
                DBObjTable *items = data->Table(DBrNItems);
                DBObjRecord *lineRec;
                DBVLineIF *lineIF = new DBVLineIF (data);
                double *padfX, *padfY;
                SHPObject *shpObject;

                for (lineRec = items->First(); lineRec != (DBObjRecord *) NULL; lineRec = items->Next()) {
                    nodeCoord = lineIF->FromCoord(lineRec);
                    vertexNum = lineIF->VertexNum(lineRec);
                    if ((padfX = (double *) calloc (sizeof (double), vertexNum + 2)) == (double *) NULL) {
                        CMmsgPrint (CMmsgSysError,"Memory allocation error.\n");
                        break;
                    }
                    if ((padfY = (double *) calloc (sizeof (double), vertexNum + 2)) == (double *) NULL) {
                        CMmsgPrint (CMmsgSysError,"Memory allocation error.\n");
                        break;
                    }
                    padfX[0] = nodeCoord.X;
                    padfY[0] = nodeCoord.Y;
                    if (vertexNum > 0) {
                        coords = lineIF->Vertexes(lineRec);
                        for (vertex = 0;vertex < vertexNum; ++vertex) {
                            padfX[vertex + 1] = coords [vertex].X;
                            padfY[vertex + 1] = coords [vertex].Y;
                        }
                    }
                    else vertex = 0;
                    nodeCoord = lineIF->ToCoord(lineRec);
                    padfX[vertex + 1] = nodeCoord.X;
                    padfY[vertex + 1] = nodeCoord.Y;

                    if ((shpObject = SHPCreateSimpleObject (SHPT_ARC,vertexNum + 2, padfX, padfY, (double *) NULL)) == (SHPObject *) NULL) {
                        CMmsgPrint (CMmsgUsrError,"SHPObject creation error");
                        break;
                    }
                    else {
                        if (SHPWriteObject (shpHandle,-1,shpObject) < 0) {
                            CMmsgPrint (CMmsgSysError, "SHPObject writing error");
                            break;
                        }
                        else
                            SHPDestroyObject (shpObject);
                    }
                }
                SHPClose (shpHandle);
                ret = lineRec == (DBObjRecord *) NULL ? DBSuccess : DBFault;
            }
            break;
        default:
            break;
    }
    return (ret);
}
