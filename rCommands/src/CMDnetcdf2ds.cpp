/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrologic Archive and Analysis System
Copyright 1994-2021, UNH - ASRC/CUNY

CMDnetcdf2ds.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input netcdf> <output datastream>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -m,--template     [template coverage]");
    CMmsgPrint(CMmsgInfo, "     -f,--field        [fieldname]");
    CMmsgPrint(CMmsgInfo, "     -h,--help");
}

int main(int argc, char *argv[]) {
    DBInt argPos, argNum = argc, ret;

    FILE *outFile;
    char *tmplName = (char *) NULL, *fieldName = (char *) NULL;
    DBObjData *grdData, *tmplData = (DBObjData *) NULL;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-m", "--template")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing network!");
                return (CMfailed);
            }
            tmplName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-f", "--field")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing fieldname!");
                return (CMfailed);
            }
            fieldName = argv[argPos];
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

    if (argNum < 2) {
        CMmsgPrint(CMmsgUsrError, "Missing input file");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    if (argNum > 3) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    outFile = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? fopen(argv[2], "w") : stdout;
    if (outFile == (FILE *) NULL) {
        CMmsgPrint(CMmsgSysError, "Output file Opening error in: %s %d", __FILE__, __LINE__);
        exit(DBFault);
    }

    grdData = new DBObjData("Noname", DBTypeGridContinuous);

    ret = DBImportNetCDF(grdData, argv[1]);

    if ((ret == DBFault) || ((grdData->Type() & DBTypeGrid) != DBTypeGrid)) {
        delete grdData;
        if (outFile != stdout) fclose(outFile);
        return (CMfailed);
    }

    if (tmplName != (char *) NULL) {
        tmplData = new DBObjData();
        if (tmplData->Read(tmplName) == DBFault) {
            delete grdData;
            if (outFile != stdout) fclose(outFile);
            delete tmplData;
            return (CMfailed);
        }
    }

    ret = RGlibRGIS2DataStream(grdData, tmplData, fieldName, outFile);
    if (tmplData != (DBObjData *) NULL) delete tmplData;
    delete grdData;
    if (outFile != stdout) fclose(outFile);
    return (ret);
}
