/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDgrdReclassDisc.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input grid> <output grid>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -f, --field     [field name]");
    CMmsgPrint(CMmsgInfo, "     -t, --title     [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject   [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain    [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version   [version]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    char *fieldName = (char *) NULL;
    DBObjData *srcData, *dstData;
 
    if (argNum < 2) goto Help;
 
    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-f", "--field")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing reclass field!");
                return (CMfailed);
            }
            fieldName = argv[argPos];
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
    if (fieldName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Missing reclass field!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    if (argNum > 3) {
        CMmsgPrint(CMmsgUsrError, "Extra arguments!");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }
    srcData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? srcData->Read(argv[1]) : srcData->Read(stdin);
    if ((ret == DBFault) || (srcData->Type() != DBTypeGridDiscrete)) {
        delete srcData;
        return (CMfailed);
    }

    if (title == (char *) NULL) {
        if ((title = (char *) malloc(strlen(srcData->Name()) + 16)) == (char *) NULL) {
            CMmsgPrint(CMmsgSysError, "Memory Allocation Error in: %s %d", __FILE__, __LINE__);
            delete srcData;
            return (CMfailed);
        }
        snprintf(title, strlen(srcData->Name()) + 16, "%s (Reclassed)", srcData->Name());
    }
    if (subject == (char *) NULL) subject = srcData->Document(DBDocSubject);
    if (domain == (char *) NULL) domain = srcData->Document(DBDocGeoDomain);
    if (version == (char *) NULL) version = srcData->Document(DBDocVersion);

    if ((dstData = DBGridToGrid(srcData, DBTypeGridDiscrete)) == (DBObjData *) NULL) {
        delete srcData;
        return (CMfailed);
    }
    dstData->Name(title);
    dstData->Document(DBDocSubject, subject);
    dstData->Document(DBDocGeoDomain, domain);
    dstData->Document(DBDocVersion, version);

    if ((ret = RGlibGridReclassDiscrete(srcData, fieldName, dstData)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ?
              dstData->Write(argv[2]) : dstData->Write(stdout);

    delete srcData;
    delete dstData;
    return (ret);
}
