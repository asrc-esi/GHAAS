/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2021, UNH - ASRC/CUNY

CMDgrdSeasonMean.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgSysError, "%s [options] <input grid> <output grid>", CMfileName(arg0));
    CMmsgPrint(CMmsgSysError, "     -a,--aggregate [avg|sum]");
    CMmsgPrint(CMmsgSysError, "     -o,--offset    [offset]");
    CMmsgPrint(CMmsgSysError, "     -e,--step      [number of steps]");
    CMmsgPrint(CMmsgSysError, "     -t,--title     [dataset title]");
    CMmsgPrint(CMmsgSysError, "     -u,--subject   [subject]");
    CMmsgPrint(CMmsgSysError, "     -d,--domain    [domain]");
    CMmsgPrint(CMmsgSysError, "     -v,--version   [version]");
    CMmsgPrint(CMmsgSysError, "     -V,--verbose");
    CMmsgPrint(CMmsgSysError, "     -h,--help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret, verbose = false;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    DBInt stepNum = 0, offset = 0, doSum = false;
    DBObjData *tsData, *data;

    if (argNum < 2) goto Help;
 
    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-a", "--aggregate")) {
            int sumCodes[] = {false, true};
            const char *sumStrs[] = {"avg", "sum", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing aggregation method!");
                return (CMfailed);
            }
            if ((doSum = CMoptLookup(sumStrs, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid aggregation method!");
                return (CMfailed);
            }
            doSum = sumCodes[doSum];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-o", "--offset")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing offset!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%d", &offset) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid number of steps!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-e", "--step")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing number of steps!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%d", &stepNum) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid number of steps!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-t", "--title")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing title!");
                return (CMfailed);
            }
            title = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-u", "--subject")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing subject!");
                return (CMfailed);
            }
            subject = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-d", "--domain")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing domain!");
                return (CMfailed);
            }
            domain = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-v", "--version")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing version!");
                return (CMfailed);
            }
            version = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-V", "--verbose")) {
            verbose = true;
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

    if (argNum > 3) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    if (verbose) RGlibPauseOpen(argv[0]);

    tsData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? tsData->Read(argv[1]) : tsData->Read(stdin);
    if ((ret == DBFault) || (tsData->Type() != DBTypeGridContinuous)) {
        delete tsData;
        return (CMfailed);
    }

    if (title == (char *) NULL) title = tsData->Name();
    if (subject == (char *) NULL) subject = tsData->Document(DBDocSubject);
    if (domain == (char *) NULL) domain = tsData->Document(DBDocGeoDomain);
    if (version == (char *) NULL) version = tsData->Document(DBDocVersion);

    data = DBGridToGrid(tsData);
    data->Name(title);
    data->Document(DBDocSubject, subject);
    data->Document(DBDocGeoDomain, domain);
    data->Document(DBDocVersion, version);

    if ((ret = RGlibSeasonMean(tsData, data, stepNum, offset, doSum)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete tsData;
    delete data;
    if (verbose) RGlibPauseClose();
    return (ret);
}
