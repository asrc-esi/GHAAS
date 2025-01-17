/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDpntGridSampling.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input grid> <output table>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -s, --sample      <sampling points or network>");
    CMmsgPrint(CMmsgInfo, "     -m, --mode        [table|attrib]");
    CMmsgPrint(CMmsgInfo, "     -n, --interpolate [surface|flat]");
    CMmsgPrint(CMmsgInfo, "     -c, --cellmode    [from|to]");
    CMmsgPrint(CMmsgInfo, "     -t, --title       [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject     [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain      [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version     [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret, mode = 0, cellMode = 0, interpolate = 0;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    char *splName = (char *) NULL;
    DBObjData *grdData, *splData, *data;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-s", "--sample")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing sampling coverage!");
                return (CMfailed);
            }
            splName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-m", "--mode")) {
            const char *modes[] = {"table", "attrib", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing mode!");
                return (CMfailed);
            }
            if ((mode = CMoptLookup(modes, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid mode!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-n", "--interpolate")) {
            const char *modes[] = {"surface", "flat", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing interpolation mode!");
                return (CMfailed);
            }
            if ((interpolate = CMoptLookup(modes, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid interpolation mode!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-c", "--cellmode")) {
            const char *modes[] = {"from", "to", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing cell mode!");
                return (CMfailed);
            }
            if ((cellMode = CMoptLookup(modes, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid cell mode!");
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
    if (splName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Sampling coverage is not specified");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    splData = new DBObjData();
    if ((splData->Read(splName) == DBFault) ||
        ((splData->Type() != DBTypeNetwork) && (splData->Type() != DBTypeVectorPoint))) {
        delete splData;
        return (CMfailed);
    }

    grdData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? grdData->Read(argv[1]) : grdData->Read(stdin);
    if ((ret == DBFault) || ((grdData->Type() & DBTypeGrid) != DBTypeGrid)) {
        delete splData;
        delete grdData;
        return (CMfailed);
    }

    if (mode == 0) {
        if (title == (char *) NULL) title = (char *) "Grid Sample";
        if (subject == (char *) NULL) subject = grdData->Document(DBDocSubject);
        if (domain == (char *) NULL) domain = splData->Document(DBDocGeoDomain);
        if (version == (char *) NULL) version = (char *) "0.01pre";

        data = new DBObjData(title, DBTypeTable);
        data->Document(DBDocGeoDomain, domain);
        data->Document(DBDocSubject, subject);
        data->Document(DBDocVersion, version);
        if ((ret = RGlibGridSampling(splData, grdData, data)) == DBSuccess)
            ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);
        delete data;
    }
    else {
        if (title != (char *) NULL) splData->Name(title);
        if (subject != (char *) NULL) splData->Document(DBDocSubject, subject);
        if (domain != (char *) NULL) splData->Document(DBDocGeoDomain, domain);
        if (version != (char *) NULL) splData->Document(DBDocVersion, version);
        RGlibGridSampling(splData, grdData, interpolate == 0 ? true : false, cellMode == 0 ? true : false);
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? splData->Write(argv[2]) : splData->Write(stdout);
    }

    delete grdData;
    delete splData;
    return (ret);
}
