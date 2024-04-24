/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDrgis2shp.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <DBif.hpp>
#include <RG.hpp>

DBInt DBNetworkExportASCIIGridDir (DBObjData *,FILE *);

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "     -o, --output [ESRI shapefile]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    char *fileName = (char *) NULL;
    int doList = false, doNum = false, doAll = true;
    DBInt layerID;
    DBObjData *data;
    DBObjRecord *layerRec;
    DBGridIF    *gridIF;
    
    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-o", "--output")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing shapefile name!");
                return (CMfailed);
            }
            fileName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage(argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && (strlen(argv[argPos]) > 1)) {
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
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin);
    if ((ret == DBFault) || ((data->Type() != DBTypeVectorPoint) && (data->Type() != DBTypeVectorLine))) {
        delete data;
        return (CMfailed);
    }
 
    switch (data->Type ()) {
        case DBTypeVectorPoint:
        case DBTypeVectorLine:  ret = DBExportShapefile (data,fileName == (char *) NULL ? "RGIS_Shapefile" : fileName); break;
        case DBTypeVectorPolygon: break;
    }

    delete data;
    return (ret);
}
