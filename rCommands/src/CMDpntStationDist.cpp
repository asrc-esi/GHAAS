/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

CMDpntStationDist.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input point> <output grid>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -e, --template  <grid or network coverage>");
    CMmsgPrint(CMmsgInfo, "     -m, --mode      [dist|ids]");
    CMmsgPrint(CMmsgInfo, "     -f, --factor    [value]");
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
    char *tmplName = (char *) NULL;
    int mode = true;
    DBFloat factor = 1.0;
    DBObjData *data, *pntData, *tmplData;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-e", "--template")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing template coverage!");
                return (CMfailed);
            }
            tmplName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-f", "--factor")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing factor!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%lf", &factor) != 1) {
                CMmsgPrint(CMmsgUsrError, "Factor scanning error!");
                return (CMfailed);
            }
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-m", "--mode")) {
            int modCodes[] = {true, false};
            const char *values[] = {"dist", "ids", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing mode!");
                return (CMfailed);
            }
            if ((mode = CMoptLookup(values, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid value!");
                return (CMfailed);
            }
            mode = modCodes[mode];
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
    if (tmplName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Template coverage is not specified");
        _CMDprintUsage (argv[0]);
        return (CMfailed);
    }

    tmplData = new DBObjData();
    if ((tmplData->Read(tmplName) == DBFault) ||
        (tmplData->Type() == DBTypeVectorPoint) ||
        (tmplData->Type() == DBTypeVectorLine) ||
        (tmplData->Type() == DBTypeVectorPolygon) ||
        (tmplData->Type() == DBTypeTable)) {
        delete tmplData;
        return (CMfailed);
    }

    pntData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? pntData->Read(argv[1]) : pntData->Read(stdin);
    if ((ret == DBFault) || (pntData->Type() != DBTypeVectorPoint)) {
        delete pntData, delete tmplData;
        return (CMfailed);
    }

    if (title == (char *) NULL) title = (char *) "Distance to Station";
    if (subject == (char *) NULL) subject = pntData->Document(DBDocSubject);
    if (domain == (char *) NULL) domain = tmplData->Document(DBDocGeoDomain);
    if (version == (char *) NULL) version = (char *) "0.01pre";

    if (tmplData->Type() == DBTypeNetwork)
        data = DBNetworkToGrid(tmplData, mode ? DBTypeGridContinuous : DBTypeGridDiscrete);
    else
        data = DBGridToGrid(tmplData, mode ? DBTypeGridContinuous : DBTypeGridDiscrete);
    delete tmplData;

    data->Name(title);
    data->Document(DBDocSubject, subject);
    data->Document(DBDocGeoDomain, domain);
    data->Document(DBDocVersion, version);

    if ((ret = DBPointToGrid(pntData, data, factor)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete pntData;
    delete data;
    return (ret);
}
