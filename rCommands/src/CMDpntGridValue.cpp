/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

CMDpntGridSampling.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <DBif.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input grid> <output table>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -c, --coordinates");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    DBCoordinate coord;
    DBObjData *data;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-c", "--coordinates")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing sampling coordinates!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%lf,%lf", &(coord.X), &(coord.Y)) != 2) {
                CMmsgPrint(CMmsgUsrError, "Invalid sampling coordinates!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
Help:   if (CMargTest (argv[argPos], "-h", "--help")) {
            _CMDprintUsage(argv[0]);
            return (DBSuccess);
        }
        if ((argv[argPos][0] == '-') && (strlen(argv[argPos]) > 1)) {
            CMmsgPrint(CMmsgUsrError, "Unknown option: %s!", argv[argPos]);
            return (CMfailed);
        }
        argPos++;
    }

    if (argNum > 2) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    data = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin);

    switch (data->Type()) {
        case DBTypeGridContinuous: {
            DBInt layerID;
            DBFloat value;
            DBGridIF *gridIF = new DBGridIF(data);
            DBObjRecord *layerRec;

            for (layerID = 0; layerID < gridIF->LayerNum(); ++layerID) {
                layerRec = gridIF->Layer(layerID);
                if (gridIF->Value(layerRec, coord, &value))
                    printf("%s: %f\n", layerRec->Name(), value);
                else
                    printf("%s: nodata\n", layerRec->Name());
            }
            delete gridIF;
        }
            break;
        case DBTypeGridDiscrete: {
            DBGridIF *gridIF = new DBGridIF(data);
            DBObjRecord *record = gridIF->GridItem(coord);
            DBObjTableField *field;
            DBInt fieldID, fieldNum = data->Table(DBrNItems)->FieldNum();

            delete gridIF;

            if (record != (DBObjRecord *) NULL) {
                for (fieldID = 0; fieldID < fieldNum; ++fieldID) {
                    field = data->Table(DBrNItems)->Field(fieldID);
                    if (DBTableFieldIsVisible(field)) {
                        switch (field->Type()) {
                            case DBTableFieldString:
                            case DBTableFieldDate:
                                printf("%s: %s\n", field->Name(), field->String(record));
                                break;
                            case DBTableFieldInt:
                                printf("%s: %d\n", field->Name(), field->Int(record));
                                break;
                            case DBTableFieldFloat:
                                printf("%s: %lf\n", field->Name(), field->Float(record));
                                break;
                            default:
                                CMmsgPrint(CMmsgAppError, "invalid field type");
                                break;
                        }
                    }
                }
            }
        }
            break;
        case DBTypeNetwork:
            break;
        default:
            break;
    }
    delete data;
    return (ret);
}

