/******************************************************************************

GHAAS RiverGIS Utilities V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

CMDnetUnaccumulate.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <cm.h>
#include <DB.hpp>
#include <RG.hpp>

static void _CMDprintUsage (const char *arg0) {
    CMmsgPrint(CMmsgInfo, "%s [options] <input grid> <output grid>", CMfileName(arg0));
    CMmsgPrint(CMmsgInfo, "     -n, --network     <network coverage>");
    CMmsgPrint(CMmsgInfo, "     -m, --mode        [rate|volume]");
    CMmsgPrint(CMmsgInfo, "     -f, --coefficient [conversion coefficient]");
    CMmsgPrint(CMmsgInfo, "     -t, --title       [dataset title]");
    CMmsgPrint(CMmsgInfo, "     -u, --subject     [subject]");
    CMmsgPrint(CMmsgInfo, "     -d, --domain      [domain]");
    CMmsgPrint(CMmsgInfo, "     -v, --version     [version]");
    CMmsgPrint(CMmsgInfo, "     -s, --shadeset    [standard|grey|blue|blue-to-red|elevation]");
    CMmsgPrint(CMmsgInfo, "     -h, --help");
}

int main(int argc, char *argv[]) {
    int argPos, argNum = argc, ret;
    float coeff;
    char *title = (char *) NULL, *subject = (char *) NULL;
    char *domain = (char *) NULL, *version = (char *) NULL;
    char *netName = (char *) NULL;
    DBInt shadeSet = DBFault;
    int areaDiv = true, coeffIsSet = false;
    DBObjData *data, *netData, *grdData;

    if (argNum < 2) goto Help;

    for (argPos = 1; argPos < argNum;) {
        if (CMargTest (argv[argPos], "-n", "--network")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing network!");
                return (CMfailed);
            }
            netName = argv[argPos];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-m", "--mode")) {
            int modeCodes[] = {true, false};
            const char *modes[] = {"rate", "volume", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing accumulation mode!");
                return (CMfailed);
            }
            if ((areaDiv = CMoptLookup(modes, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid \"unaccumulation\" mode!");
                return (CMfailed);
            }
            areaDiv = modeCodes[areaDiv];
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) break;
            continue;
        }
        if (CMargTest (argv[argPos], "-f", "--coefficient")) {
            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing coefficient!");
                return (CMfailed);
            }
            if (sscanf(argv[argPos], "%f", &coeff) != 1) {
                CMmsgPrint(CMmsgUsrError, "Invalid coefficient");
                return (CMfailed);
            }
            coeffIsSet = true;
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
        if (CMargTest (argv[argPos], "-s", "--shadeset")) {
            int shadeCodes[] = {DBDataFlagDispModeContStandard,
                                DBDataFlagDispModeContGreyScale,
                                DBDataFlagDispModeContBlueScale,
                                DBDataFlagDispModeContBlueRed,
                                DBDataFlagDispModeContElevation};
            const char *shadeSets[] = {"standard", "grey", "blue", "blue-to-red", "elevation", (char *) NULL};

            if ((argNum = CMargShiftLeft(argPos, argv, argNum)) <= argPos) {
                CMmsgPrint(CMmsgUsrError, "Missing shadeset!");
                return (CMfailed);
            }
            if ((shadeSet = CMoptLookup(shadeSets, argv[argPos], true)) == DBFault) {
                CMmsgPrint(CMmsgUsrError, "Invalid shadeset!");
                return (CMfailed);
            }
            shadeSet = shadeCodes[shadeSet];
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
    if (netName == (char *) NULL) {
        CMmsgPrint(CMmsgUsrError, "Network is not specified");
        _CMDprintUsage (argv[0]);
       return (CMfailed);
    }

    netData = new DBObjData();
    if ((netData->Read(netName) == DBFault) || (netData->Type() != DBTypeNetwork)) {
        delete netData;
        return (CMfailed);
    }

    grdData = new DBObjData();
    ret = (argNum > 1) && (strcmp(argv[1], "-") != 0) ? grdData->Read(argv[1]) : grdData->Read(stdin);
    if ((ret == DBFault) || (grdData->Type() != DBTypeGridContinuous)) {
        delete netData;
        delete grdData;
        return (CMfailed);
    }

    if (title == (char *) NULL) title = (char *) "Unaccumulated Grid";
    if (subject == (char *) NULL) subject = grdData->Document(DBDocSubject);
    if (domain == (char *) NULL) domain = netData->Document(DBDocGeoDomain);
    if (version == (char *) NULL) version = (char *) "0.01pre";

    data = DBNetworkToGrid(netData, DBTypeGridContinuous);
    data->Name(title);
    data->Document(DBDocSubject, subject);
    data->Document(DBDocGeoDomain, domain);
    data->Document(DBDocVersion, version);
    if (shadeSet != DBFault) {
        data->Flags(DBDataFlagDispModeContShadeSets, DBClear);
        data->Flags(shadeSet, DBSet);
    }
    if (coeffIsSet != true) { coeff = areaDiv ? 1000000.0 : 1.0; }

    if ((ret = RGlibNetworkUnaccumulate(netData, grdData, (DBFloat) coeff, areaDiv, data)) == DBSuccess)
        ret = (argNum > 2) && (strcmp(argv[2], "-") != 0) ? data->Write(argv[2]) : data->Write(stdout);

    delete data;
    delete netData;
    delete grdData;
    return (ret);
}
