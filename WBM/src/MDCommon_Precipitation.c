/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDCommon_Precipitation.c

bfekete@ccny.cuny.edu

******************************************************************************/

#include <math.h>
#include <MF.h>
#include <MD.h>

static int _MDInCommon_PrecipitationDailyID     = MFUnset;
static int _MDInCommon_PrecipitationMonthlyID   = MFUnset;
static int _MDInCommon_PrecipitationReferenceID = MFUnset;
static int _MDInCommon_PrecipitationWetDaysID   = MFUnset;
static int _MDInCommon_PrecipitationFractionID  = MFUnset;
static int _MDOutCommon_PrecipitationID         = MFUnset;

static void _MDPrecipDownscale (int itemID) {
// Input 
	float precipDaily   = MFVarGetFloat (_MDInCommon_PrecipitationDailyID,     itemID, 0.0); // Daily precipitation in mm/dt
	float precipMonthly = MFVarGetFloat (_MDInCommon_PrecipitationMonthlyID,   itemID, 0.0); // Monthly precipitation in mm/dt
	float precipRef     = MFVarGetFloat (_MDInCommon_PrecipitationReferenceID, itemID, 0.0); // Monthly reference precipitation in mm/dt
// Output 
	float precip;

	precip = (precipMonthly > 0.0) && (precipRef > 0.0) ? precipDaily * precipRef / (precipMonthly) : 0.0;
	MFVarSetFloat (_MDOutCommon_PrecipitationID, itemID, precip);
}

static void _MDPrecipFraction (int itemID) {
// Model
	int nDays    = MFDateGetMonthLength ();
// Input 
	float precipIn   = MFVarGetFloat (_MDInCommon_PrecipitationMonthlyID,  itemID, 0.0);
	float precipFrac = MFVarGetFloat (_MDInCommon_PrecipitationFractionID, itemID, 1.0 / nDays);
// Output 
	float precipOut;

	precipOut = precipFrac * precipIn *  nDays;
	if (precipOut < 0.0){ CMmsgPrint (CMmsgUsrError, "Precip negative! itemID=%d precipIn=%f precipFrac =%fprecipFrac", itemID, precipIn, precipFrac);}
	MFVarSetFloat (_MDOutCommon_PrecipitationID, itemID, precipOut);
}

bool MDEvent (int nSteps,int nEvents,int step) {
  	bool inv = false;
	int event;
	float freq;

	if (nSteps == nEvents) return (true);

	if (nEvents > nSteps / 2) { nEvents = nSteps - nEvents; inv = true; }
		
  	freq = (float) nSteps / (float) nEvents;
	for (event = 0;event < step;++event)
		if ((int) (rint (event * freq + freq / 2.0)) == step) return (inv ? false : true);

	return (inv ? true : false);
}

static void _MDPrecipWetDays (int itemID) {
// Model
	int day   = MFDateGetCurrentDay ();
	int nDays = MFDateGetMonthLength ();
// Input 
	float precipIn = MFVarGetFloat (_MDInCommon_PrecipitationMonthlyID, itemID,  0.0);
	int  wetDays   = MFVarGetInt   (_MDInCommon_PrecipitationWetDaysID, itemID, 31.0);
// Output 
	float precipOut; 

	precipOut = MDEvent (nDays,wetDays,day) ? precipIn * (float) nDays / (float) wetDays : 0.0;

	MFVarSetFloat (_MDOutCommon_PrecipitationID, itemID, precipOut);
}

enum { MDhelp, MDinput, MDdownscale, MDfraction, MDwetdays };

int MDCommon_PrecipitationDef () {
	int optID = MDinput;
	const char *optStr;
	const char *options [] = { MFhelpStr, MFinputStr, "downscale", "fraction", "wetdays", (char *) NULL };

	if (_MDOutCommon_PrecipitationID != MFUnset) return (_MDOutCommon_PrecipitationID);

	MFDefEntering ("Precipitation");
	if ((optStr = MFOptionGet (MDVarCommon_Precipitation)) != (char *) NULL) optID = CMoptLookup (options,optStr,true);
	switch (optID) {
		default:
		case MDhelp:  MFOptionMessage (MDVarCommon_Precipitation, optStr, options); return (CMfailed);
		case MDinput: _MDOutCommon_PrecipitationID = MFVarGetID (MDVarCommon_Precipitation, "mm", MFInput, MFFlux, MFBoundary); break;
		case MDdownscale:
			if (((_MDInCommon_PrecipitationMonthlyID   = MFVarGetID (MDVarCommon_PrecipitationMonthly,   "mm", MFInput,  MFFlux,  MFBoundary)) == CMfailed) ||
				((_MDInCommon_PrecipitationDailyID     = MFVarGetID (MDVarCommon_PrecipitationDaily,     "mm", MFInput,  MFFlux,  MFBoundary)) == CMfailed) ||
                ((_MDInCommon_PrecipitationReferenceID = MFVarGetID (MDVarCommon_PrecipitationReference, "mm", MFInput,  MFFlux,  MFBoundary)) == CMfailed) ||
                ((_MDOutCommon_PrecipitationID         = MFVarGetID (MDVarCommon_Precipitation,          "mm", MFOutput, MFFlux,  MFBoundary)) == CMfailed) ||
                (MFModelAddFunction (_MDPrecipDownscale) == CMfailed)) return (CMfailed);
			break;
		case MDfraction:
			if (((_MDInCommon_PrecipitationMonthlyID  = MFVarGetID (MDVarCommon_PrecipitationMonthly,    "mm", MFInput,  MFFlux,  MFBoundary)) == CMfailed) ||
                ((_MDInCommon_PrecipitationFractionID = MFVarGetID (MDVarCommon_PrecipitationFraction,   "mm", MFInput,  MFState, MFBoundary)) == CMfailed) ||
                ((_MDOutCommon_PrecipitationID        = MFVarGetID (MDVarCommon_Precipitation,           "mm", MFOutput, MFFlux,  MFBoundary)) == CMfailed) ||
                (MFModelAddFunction (_MDPrecipFraction) == CMfailed)) return (CMfailed);
			break;
		case MDwetdays:
			if (((_MDInCommon_PrecipitationWetDaysID    = MDCommon_WetDaysDef ()) == CMfailed) ||
                ((_MDInCommon_PrecipitationMonthlyID    = MFVarGetID (MDVarCommon_PrecipMonthly,         "mm", MFInput,  MFFlux,  MFBoundary)) == CMfailed) ||
                ((_MDOutCommon_PrecipitationID          = MFVarGetID (MDVarCommon_Precipitation,         "mm", MFOutput, MFFlux,  MFBoundary)) == CMfailed) ||
                (MFModelAddFunction (_MDPrecipWetDays) == CMfailed)) return (CMfailed);
			break;
	}
	MFDefLeaving ("Precipitation");
	return (_MDOutCommon_PrecipitationID);
}
