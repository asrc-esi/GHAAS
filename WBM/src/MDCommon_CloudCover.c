/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDCommon_CloudCover.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <string.h>
#include <math.h>
#include <MF.h>
#include <MD.h>

//Input
static int _MDInCommon_SolarRadID    = MFUnset;
static int _MDInCommon_GrossRadID    = MFUnset;
//Output
static int _MDOutCommon_CloudCoverID = MFUnset;

static void _MDCommon_CloudCover(int itemID) {
// Input
    float grossRad = MFVarGetFloat(_MDInCommon_GrossRadID, itemID, 0.0); // Gross/Clear Sky radiation W/m2
    float solarRad = MFVarGetFloat(_MDInCommon_SolarRadID, itemID, 0.0); // Solar radiation W/m2
// Output
    float cloudCover;
// Local
    float LHS;
    float a;
    float b;
    float c;

    LHS = solarRad / grossRad;
    a = 0.458;
    b = 0.340;
    c = LHS - 0.803;

    cloudCover = 100.0 * (-b + pow((pow(b,2) - (4 * a * c)),0.5)) / (2 * a);

    if (cloudCover <   0.0) cloudCover =   0.0;
    if (cloudCover > 100.0) cloudCover = 100.0;

    MFVarSetFloat(_MDOutCommon_CloudCoverID, itemID, cloudCover);
}

enum { MDhelp, MDinput, MDcalculate }; // This is different from the standard MFcalcOptions [help, none, input, caclulate] 

int MDCommon_CloudCoverDef() {
    int optID = MDinput;
    const char *optStr;

    if (_MDOutCommon_CloudCoverID != MFUnset) return (_MDOutCommon_CloudCoverID);
 
    MFDefEntering("CloudCover");
    if ((optStr = MFOptionGet (MDVarCommon_CloudCover)) != (char *) NULL) optID = CMoptLookup(MFsourceOptions, optStr, true);
    switch (optID) {
        default:
        case MDhelp:  MFOptionMessage (MDVarCommon_CloudCover, optStr, MFsourceOptions); return (CMfailed);
        case MDinput: _MDOutCommon_CloudCoverID = MFVarGetID(MDVarCommon_CloudCover, "fraction", MFInput, MFState, MFBoundary); break;
        case MDcalculate:
            if (((_MDInCommon_GrossRadID    = MDCommon_GrossRadDef()) == CMfailed) ||
                ((_MDInCommon_SolarRadID    = MFVarGetID (MDVarCore_SolarRadiation, "W/m^2", MFInput,  MFState, MFBoundary)) == CMfailed) ||
                ((_MDOutCommon_CloudCoverID = MFVarGetID (MDVarCommon_CloudCover,   "%",     MFOutput, MFState, MFBoundary)) == CMfailed) ||
                ((MFModelAddFunction (_MDCommon_CloudCover) == CMfailed))) return (CMfailed);
            break;
    }
    MFDefLeaving("CloudCover");
    return (_MDOutCommon_CloudCoverID);
}
