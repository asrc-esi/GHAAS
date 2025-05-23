/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDParam_LandCoverMapping.c

dominik.wisser@unh.edu

*******************************************************************************/

#include <MF.h>
#include <MD.h>

static int _MDOutCoverID      = MFUnset;
static int _MDInCroplandPctID = MFUnset;

static int _MDInSAGEVegID;

static void _MDLCSAGEVegToCover (int itemID) {
	switch (MFVarGetInt (_MDInSAGEVegID,itemID, 15)) {
		case  1: MFVarSetInt (_MDOutCoverID,itemID, 8); break;
		case  2:
		case  3:
		case  4:
		case  5:
		case  6: MFVarSetInt (_MDOutCoverID,itemID, 2); break;
		case  7:
		case  8: MFVarSetInt (_MDOutCoverID,itemID, 1); break;
		case  9: MFVarSetInt (_MDOutCoverID,itemID, 2); break;
		case 10: MFVarSetInt (_MDOutCoverID,itemID, 3); break;
		case 11: MFVarSetInt (_MDOutCoverID,itemID, 4); break;
		case 12:
		case 13: MFVarSetInt (_MDOutCoverID,itemID, 3); break;
		case 14:
		case 15: MFVarSetInt (_MDOutCoverID,itemID, 7); break;
	}
}

static int _MDInTEMVegID;

static void _MDLCTEMVegToCover (int itemID) {
	int wetland = 3;
	
	switch (MFVarGetInt (_MDInTEMVegID,itemID, -36)) {
		case   4: // Conifer forest
		case   9: MFVarSetInt (_MDOutCoverID,itemID, 0); break;
		case   8: // Broadleaf forest
		case  10:
		case  18:
		case  16: 
		case  33: MFVarSetInt (_MDOutCoverID,itemID, 1); break;
		case  14: // Savannah
		case  15:
		case  19:
		case  27:
		case  31:
		case  35: MFVarSetInt (_MDOutCoverID,itemID, 2); break;
		case   6: // Grassland
		case  12:
		case  13: MFVarSetInt (_MDOutCoverID,itemID, 3); break;
		case   2: // Tundra
		case   3: MFVarSetInt (_MDOutCoverID,itemID, 4); break;
				  // Cultivation
				  // Desert
		case  21: MFVarSetInt (_MDOutCoverID,itemID, 6); break;
		case -36: // Water
		case -37: MFVarSetInt (_MDOutCoverID,itemID, 7); break;
		case  11: // Wetland forest
		case  17:
		case  20:
		case  25:
		case  29: if (wetland == 1) MFVarSetMissingVal (_MDOutCoverID,itemID);
					 else MFVarSetInt (_MDOutCoverID,itemID,wetland == 2 ? 7 : 1); break;
					 // Wetland Savannah
		case   5: if (wetland == 1) MFVarSetMissingVal (_MDOutCoverID,itemID);
					 else MFVarSetInt (_MDOutCoverID,itemID,wetland == 2 ? 7 : 2); break;
		case   7: // Wetland Tundra
		case  22:
		case  23:
		case  24:
		case  26:
		case  28:
		case  30: if (wetland == 1) MFVarSetMissingVal (_MDOutCoverID,itemID);
					 else MFVarSetInt (_MDOutCoverID,itemID,wetland == 2 ? 7 : 4); break;
		case   1:
		default:	MFVarSetMissingVal (_MDOutCoverID,itemID);
	}
}

enum { MDhelp, MDinput, MDLCSAGEVeg, MDLCTEMVeg };

int MDParam_LandCoverMappingDef () {
	int optID = MDinput;
	const char *optStr;
	const char *options [] = { MFhelpStr, MFinputStr, "SAGEVeg", "TEMVeg", (char *) NULL };

	if (_MDOutCoverID != MFUnset) return (_MDOutCoverID);

	MFDefEntering ("Landcover");
	if ((optStr = MFOptionGet (MDVarCore_LandCoverWBM)) != (char *) NULL) optID = CMoptLookup (options,optStr,true);
	switch (optID) {
		default:
		case MDhelp:  MFOptionMessage (MDVarCore_LandCoverWBM, optStr, options); return (CMfailed);
		case MDinput: _MDOutCoverID = MFVarGetID (MDVarCore_LandCoverWBM, MFNoUnit, MFInput, MFState, MFBoundary); break;
		case MDLCSAGEVeg:
			  if (((_MDInSAGEVegID  = MFVarGetID (MDVarCore_LandCoverSAGE, MFNoUnit, MFInput, MFState, MFBoundary)) == CMfailed) ||
                  ((_MDOutCoverID   = MFVarGetID (MDVarCore_LandCoverWBM, MFNoUnit,  MFByte,  MFState, MFBoundary)) == CMfailed) ||
                  (MFModelAddFunction (_MDLCSAGEVegToCover) == CMfailed)) return (CMfailed);
			break;
		case MDLCTEMVeg:
			  if (((_MDInTEMVegID   = MFVarGetID (MDVarCore_LandCoverTEM, MFNoUnit, MFInput, MFState, MFBoundary)) == CMfailed) ||
                  ((_MDOutCoverID   = MFVarGetID (MDVarCore_LandCoverWBM, MFNoUnit, MFByte,  MFState, MFBoundary)) == CMfailed) ||
                  (MFModelAddFunction (_MDLCTEMVegToCover) == CMfailed)) return (CMfailed);
			break;
	}
	MFDefLeaving ("Landcover");
	return (_MDOutCoverID);
}
