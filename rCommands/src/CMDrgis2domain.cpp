/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - CCNY

CMDrgis2domain.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <DBif.hpp>
#include <MF.h>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input rgisdata> <output domain>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -l, --lengthcorrection");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    FILE *outFile;
    DBInt argPos, argNum = argc, ret;
    int objID, size;
    DBFloat lCorrection = 1.0;
    MFDomain_t *domain = (MFDomain_t *) NULL;
    DBCoordinate coord;
    DBObjRecord *objRec;
    DBObjData *data;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-l", "--lengthcorrection")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing length correction!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%lf", &lCorrection) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid length correction!");
                return (CMfailed);
            }
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

    outFile = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? fopen(argv[2], "w") : stdout;
    if (outFile == (FILE *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Output file Opening error in: %s", CMfileName(argv[0]));
        _CMDprintUsage (argv[0]);
        return(DBFault);
    }

    data = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? data->Read(argv[1]) : data->Read(stdin);
    if (ret == DBFault) {
        CMmsgPrint(CMmsgUsrError, "Domain reading error in: %s", CMfileName(argv[0]));
        _CMDprintUsage (argv[0]);
        if (outFile != stdout) fclose (outFile);
        return(DBFault);

    }

    if ((domain = (MFDomain_t *) calloc(1, sizeof(MFDomain_t))) != (MFDomain_t *) NULL) {
        domain->Objects = (MFObject_t *) NULL;
        switch (data->Type()) {
            case DBTypeVectorPoint: {
                DBVPointIF *pntIF = new DBVPointIF(data);

                domain->ObjNum = pntIF->ItemNum();
                if ((domain->Objects = (MFObject_t *) calloc(domain->ObjNum, sizeof(MFObject_t))) ==
                    (MFObject_t *) NULL) {
                    CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
                    MFDomainFree(domain);
                    delete pntIF;
                    goto Stop;
                }
                for (objID = 0; objID < domain->ObjNum; ++objID) {
                    objRec = pntIF->Item(objID);
                    coord = pntIF->Coordinate(objRec);
                    domain->Objects[objID].ID = objRec->RowID();
                    domain->Objects[objID].DLinkNum = 0;
                    domain->Objects[objID].ULinkNum = 0;
                    domain->Objects[objID].DLinks = (size_t *) NULL;
                    domain->Objects[objID].ULinks = (size_t *) NULL;
                    domain->Objects[objID].XCoord = domain->Objects[objID].Lon = coord.X;
                    domain->Objects[objID].YCoord = domain->Objects[objID].Lat = coord.Y;
                    domain->Objects[objID].Area = 0.0;
                    domain->Objects[objID].Length = 0.0;
                }
                delete pntIF;
            } break;
            case DBTypeGridContinuous:
            case DBTypeGridDiscrete: {
            } break;
            case DBTypeNetwork: {
                DBInt dir;
                DBObjRecord *nextCell;
                DBNetworkIF *netIF = new DBNetworkIF(data);
                domain->ObjNum = netIF->CellNum();
                if ((domain->Objects = (MFObject_t *) calloc(domain->ObjNum, sizeof(MFObject_t))) ==
                    (MFObject_t *) NULL) {
                    CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
                    MFDomainFree(domain);
                    delete netIF;
                    goto Stop;
                }
                for (objID = 0; objID < domain->ObjNum; ++objID) {
                    domain->Objects[objID].DLinks = (size_t *) NULL;
                    domain->Objects[objID].ULinks = (size_t *) NULL;
                }
                for (objID = 0; objID < domain->ObjNum; ++objID) {
                    objRec = netIF->Cell(objID);
                    coord = netIF->Center(objRec);
                    domain->Objects[objID].ID = objRec->RowID();
                    domain->Objects[objID].DLinkNum = 0;
                    domain->Objects[objID].ULinkNum = 0;
                    domain->Objects[objID].XCoord = domain->Objects[objID].Lon = coord.X;
                    domain->Objects[objID].YCoord = domain->Objects[objID].Lat = coord.Y;
                    domain->Objects[objID].Area = netIF->CellArea(objRec);
                    domain->Objects[objID].Length = netIF->CellLength(objRec) * lCorrection;
                    if ((nextCell = netIF->ToCell(objRec)) != (DBObjRecord *) NULL) {
                        size = (domain->Objects[objID].DLinkNum + 1) * sizeof(size_t);
                        if ((domain->Objects[objID].DLinks = (size_t *) realloc(domain->Objects[objID].DLinks, size)) ==
                            (size_t *) NULL) {
                            CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
                            MFDomainFree(domain);
                            delete netIF;
                            goto Stop;
                        }
                        domain->Objects[objID].DLinks[domain->Objects[objID].DLinkNum] = nextCell->RowID();
                        domain->Objects[objID].DLinkNum++;
                    }
                    for (dir = 0; dir < 8; ++dir)
                        if ((nextCell = netIF->FromCell(objRec, 0x01 << dir)) != (DBObjRecord *) NULL) {
                            size = (domain->Objects[objID].ULinkNum + 1) * sizeof(size_t);
                            if ((domain->Objects[objID].ULinks = (size_t *) realloc(domain->Objects[objID].ULinks,
                                                                                    size)) == (size_t *) NULL) {
                                CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
                                MFDomainFree(domain);
                                delete netIF;
                                goto Stop;
                            }
                            domain->Objects[objID].ULinks[domain->Objects[objID].ULinkNum] = nextCell->RowID();
                            domain->Objects[objID].ULinkNum++;
                        }
                }
                delete netIF;
            } break;
        }
        ret = MFDomainWrite(domain, outFile);
    }
    Stop:
    delete data;
    if (outFile != stdout) fclose(outFile);
    return (ret);
}
