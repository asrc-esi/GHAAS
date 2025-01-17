/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDIrrigation_IrrArea.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <MF.h>
#include <MD.h>

// Input
static int _MDInIrrAreaFracSeason1ID = MFUnset;
static int _MDInIrrAreaFracSeason2ID = MFUnset;
static int _MDInGrowingSeason1ID     = MFUnset;
static int _MDInGrowingSeason2ID     = MFUnset;
// Output
static int _MDOutIrrigatedAreaFracID = MFUnset; 

static void _MDIrrigatedAreaIWMI (int itemID) {
// Input
	float irrAreaFracSeason1 = MFVarGetFloat(_MDInIrrAreaFracSeason1ID, itemID, 0.0);
	float irrAreaFracSeason2 = MFVarGetFloat(_MDInIrrAreaFracSeason2ID, itemID, 0.0);
	float Season1Doy         = MFVarGetFloat(_MDInGrowingSeason1ID,     itemID, 100);
	float Season2Doy         = MFVarGetFloat(_MDInGrowingSeason2ID,     itemID, 250);
// Output
	float irrAreaFrac;	

	if (Season1Doy < Season2Doy) {
		if      (MFDateGetDayOfYear () < Season1Doy) irrAreaFrac = irrAreaFracSeason2;
		else if (MFDateGetDayOfYear () < Season2Doy) irrAreaFrac = irrAreaFracSeason1;
		else irrAreaFrac = irrAreaFracSeason2;
	} else {
		if (MFDateGetDayOfYear() <= Season1Doy && MFDateGetDayOfYear() > Season2Doy) irrAreaFrac = irrAreaFracSeason1;
		else irrAreaFrac = irrAreaFracSeason2;
	}
	MFVarSetFloat(_MDOutIrrigatedAreaFracID, itemID, irrAreaFrac);
}

enum { MDhelp, MDfao, MDiwmi };

int MDIrrigation_IrrAreaDef () {
	int optID = MDfao, irrOptID = MFnone;
	const char *optStr;
	const char *options [] = { MFhelpStr, "FAO", "IWMI", (char *) NULL };

	if (_MDOutIrrigatedAreaFracID != MFUnset) return (_MDOutIrrigatedAreaFracID);

	if ((optStr = MFOptionGet (MDOptConfig_Irrigation)) != (char *) NULL) irrOptID = CMoptLookup (MFcalcOptions,optStr,true);
	if (irrOptID != MFcalculate) return (_MDOutIrrigatedAreaFracID);

	MFDefEntering ("Irrigated Area");
	if ((optStr = MFOptionGet (MDOptIrrigation_AreaMap)) != (char *) NULL) optID = CMoptLookup (options, optStr, true);

	switch (optID) {
        default:
		case MDhelp:  MFOptionMessage (MDOptIrrigation_AreaMap, optStr, options); return (CMfailed);
      case MDfao:
      	if ((_MDOutIrrigatedAreaFracID = MFVarGetID (MDVarIrrigation_AreaFraction, MFNoUnit, MFInput, MFState, MFBoundary)) == CMfailed) return (CMfailed);
			break;
		case MDiwmi:
		    if (((_MDInIrrAreaFracSeason1ID = MFVarGetID (MDVarIrrigation_AreaFractionSeason1, MFNoUnit, MFInput,  MFState, MFBoundary)) == CMfailed) ||
                ((_MDInIrrAreaFracSeason2ID = MFVarGetID (MDVarIrrigation_AreaFractionSeason2, MFNoUnit, MFInput,  MFState, MFBoundary)) == CMfailed) ||
                ((_MDInGrowingSeason1ID     = MFVarGetID (MDVarIrrigation_GrowingSeason1Start, "DoY",    MFInput,  MFState, MFBoundary)) == CMfailed) ||
                ((_MDInGrowingSeason2ID     = MFVarGetID (MDVarIrrigation_GrowingSeason2Start, "DoY",    MFInput,  MFState, MFBoundary)) == CMfailed) ||
				((_MDOutIrrigatedAreaFracID = MFVarGetID (MDVarIrrigation_AreaFraction,        MFNoUnit, MFOutput, MFState, MFBoundary)) == CMfailed) ||
                (MFModelAddFunction (_MDIrrigatedAreaIWMI) == CMfailed)) return (CMfailed);
		    break;
	}
	MFDefLeaving ("IrrigatedArea");
	return (_MDOutIrrigatedAreaFracID);
}
