/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDgrdZoneStats.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input grid> <output table>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -z, --zonegrid  <discrete grid coverage>");
    CMmsgPrint(CMmsgInfo, "     -t, --title     [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject   [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain    [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version   [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    char *title  = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    char *zoneGridName = (char *) NULL;
    DBObjData *data, *zGrdData, *wGrdData;

    if (argNum < 2) goto Help;
 
    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-z", "--zonegrid")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing zonegrid!");
                return (CMfailed);
            }
            zoneGridName = argv[argPos];
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

    if (zoneGridName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Zone grid is not specified");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    zGrdData = new DBObjData();
    if ((zGrdData->Read(zoneGridName) == DBFault) || (zGrdData->Type() != DBTypeGridDiscrete)) {
        delete zGrdData;
        return (CMfailed);
    }

    wGrdData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? wGrdData->Read(argv[1]) : wGrdData->Read(stdin);
    if ((ret == DBFault) || (wGrdData->Type() != DBTypeGridContinuous)) {
        delete zGrdData;
        delete wGrdData;
        return (CMfailed);
    }

    if (title   == (char *) NULL) title   = (char *) "Zone Statistics";
    if (subject == (char *) NULL) subject = (char *) "Basin Statistics";
    if (domain  == (char *) NULL) domain  = zGrdData->Document(DBDocGeoDomain);
    if (version == (char *) NULL) version = (char *) "0.01pre";

    data = new DBObjData(title, DBTypeTable);
    data->Document(DBDocSubject,   subject);
    data->Document(DBDocGeoDomain, domain);
    data->Document(DBDocVersion,   version);

    if ((ret = RGlibGridZoneStatistics(zGrdData, wGrdData, data)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    delete zGrdData;
    delete wGrdData;
    return (ret);
}
