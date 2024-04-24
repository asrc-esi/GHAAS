/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDnetPouorElevation.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input network> <output network>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -e, --elevation   [elevation field]");
    CMmsgPrint(CMmsgInfo, "     -p, --pourelev    [pour elevation field]");
    CMmsgPrint(CMmsgInfo, "     -t, --title       [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject     [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain      [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version     [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    char *elev  = (char *) "Elevation", *pour    = (char *) "PourElev";
    char *title = (char *) NULL,        *subject = (char *) NULL;
    char *domain = (char *) NULL,       *version = (char *) NULL;
    DBObjData *netData;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-e", "--elevation")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing elevation field!");
                return (CMfailed);
            }
            elev = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-p", "--pour")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing pour elevation field!");
                return (CMfailed);
            }
            pour = argv[argPos];
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
    netData = new DBObjData();

    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? netData->Read(argv[1]) : netData->Read(stdin);
    if (netData->Type() != DBTypeNetwork) {
        CMmsgPrint(CMmsgUsrError, "Not a network input!");
        _CMDprintUsage (argv[0]);
        delete netData;
        return (CMfailed);
    }
    if (title   != (char *) NULL) netData->Name(title);
    if (subject != (char *) NULL) netData->Document(DBDocSubject, subject);
    if (domain  != (char *) NULL) netData->Document(DBDocGeoDomain, domain);
    if (version != (char *) NULL) netData->Document(DBDocVersion, version);

    if ((ret = RGlibNetworkPourElevation (netData,elev,pour)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? netData->Write(argv[2]) : netData->Write(stdout);

    delete netData;
    return (ret);
}
