/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDCore_SoilMoistChg.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <MF.h>
#include <MD.h>

// Input
static int _MDInRainSoilMoistID     = MFUnset;
static int _MDInRainSoilMoistChgID  = MFUnset;
static int _MDInIrrSoilMoistID      = MFUnset;
static int _MDInIrrSoilMoistChgID   = MFUnset;
static int _MDInSoilAvailWaterCapID = MFUnset;
// Output
static int _MDOutSoilMoistID        = MFUnset;
static int _MDOutSoilMoistChgID     = MFUnset;
static int _MDOutRelSoilMoistID     = MFUnset;

static void _MDSoilMoistChg (int itemID) {	
// Input
	float sMoist            = MFVarGetFloat (_MDInRainSoilMoistID,     itemID, 0.0); // Non-irrigated soil moisture [mm/dt]
	float sMoistChg         = MFVarGetFloat (_MDInRainSoilMoistChgID,  itemID, 0.0); // Non-irrigated soil moisture change [mm/dt]
	float soilAvailWaterCap = MFVarGetFloat (_MDInSoilAvailWaterCapID, itemID, 0.0); // Available water capacity [mm]
	
	if (_MDInIrrSoilMoistID    != MFUnset) sMoist    += MFVarGetFloat (_MDInIrrSoilMoistID,    itemID, 0.0);
	if (_MDInIrrSoilMoistChgID != MFUnset) sMoistChg += MFVarGetFloat (_MDInIrrSoilMoistChgID, itemID, 0.0);

	MFVarSetFloat (_MDOutSoilMoistID,    itemID, sMoist);
	MFVarSetFloat (_MDOutSoilMoistChgID, itemID, sMoistChg);
	MFVarSetFloat (_MDOutRelSoilMoistID, itemID, CMmathEqualValues (soilAvailWaterCap, 0.0) ? 0.0 : sMoist / soilAvailWaterCap);
}

int MDCore_SoilMoistChgDef () {
	int ret;
	if (_MDOutSoilMoistChgID != MFUnset) return (_MDOutSoilMoistChgID);

	MFDefEntering ("Soil Moisture");

	if (((ret = MDIrrigation_GrossDemandDef()) != MFUnset) &&
        ((ret == CMfailed) ||
         ((_MDInIrrSoilMoistID     = MDIrrigation_SoilMoistDef ())    == CMfailed) ||
         ((_MDInIrrSoilMoistChgID  = MDIrrigation_SoilMoistChgDef ()) == CMfailed)))
	     return (CMfailed);
	if (((_MDInSoilAvailWaterCapID = MDCore_SoilAvailWaterCapDef())   == CMfailed) ||
        ((_MDInRainSoilMoistChgID  = MDCore_RainSMoistChgDef ())      == CMfailed) ||
        ((_MDInRainSoilMoistID     = MFVarGetID (MDVarCore_RainSoilMoisture,    "mm", MFInput,  MFState, MFInitial))  == CMfailed) ||
        ((_MDOutSoilMoistID        = MFVarGetID (MDVarCore_SoilMoisture,        "mm", MFOutput, MFState, MFBoundary)) == CMfailed) ||
        ((_MDOutSoilMoistChgID     = MFVarGetID (MDVarCore_SoilMoistChange,     "mm", MFOutput, MFState, MFBoundary)) == CMfailed) ||
        ((_MDOutRelSoilMoistID     = MFVarGetID (MDVarCore_RelSoilMoisture,     "mm", MFOutput, MFState, MFInitial))  == CMfailed) ||
        (MFModelAddFunction (_MDSoilMoistChg) == CMfailed)) return (CMfailed);
	MFDefLeaving ("Soil Moisture");
	return (_MDOutSoilMoistChgID);
}
