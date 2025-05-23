/******************************************************************************

GHAAS Water Balance/Transport Model
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

MDCommon_GrossRad.c

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <math.h>
#include <MF.h>
#include <MD.h>

#define  DTOR 0.01745329252
static float _MDGrossRadStdTAU = 1.0;

static int _MDOutCommon_GrossRadID = MFUnset;

static void _MDCommon_GrossRadianceStd (int itemID) {
// Input
	int   day    = MFDateGetDayOfYear ();
	float lambda = MFModelGetLatitude (itemID) * DTOR;
// Output
	float  grossRad;
// Local
	int   hour;
	double eta, sigma, sinphi, sp, sbb;

	sp = 1360.0; // Solar constant in W/m2
	grossRad = 0;
	sigma = -23.4 * cos (2.0 * M_PI * (day + 11.0) / 365.25) * DTOR;
	for (hour = 0;hour < 24; hour++) {
		eta = (hour + 1) * M_PI / 12.0;
		sinphi = sin (lambda) * sin (sigma) + cos (lambda) * cos (sigma) * cos (eta);
		sbb = sp * sinphi * pow ((double) _MDGrossRadStdTAU,(double) (1.0 / sinphi));
		if (sbb > 0) grossRad += sbb;
	}
	MFVarSetFloat (_MDOutCommon_GrossRadID,  itemID, grossRad / 24.0);
}

static void _MDCommon_GrossRadianceOtto (int itemID) {
// Input
	int   day    = MFDateGetDayOfYear ();
	float lambda = MFModelGetLatitude (itemID) * DTOR;
// Output
	float  grossRad; // W/m2
// Local
	int   hour;
	double eta, sigma,sinphi,sp,sbb,sotd;

	sp = 1360.0; // Solar constant in W/m2
	grossRad = 0.0;
	sigma = -23.4856 * cos (2.0 * M_PI * (day + 11.0) / 365.25) * DTOR;
	for (hour = 0;hour < 24;hour++) {
		eta = (double) ((hour + 1)) * M_PI / 12.0;
		sinphi = sin (lambda) * sin (sigma) + cos (lambda) * cos (sigma) * cos (eta);
 		sotd = 1 - (0.016729 * cos (0.9856 * (day - 4.0) * DTOR));
		sbb = sp * sinphi / pow (sotd,2.0);
		if (sbb >= 0) grossRad += sbb;
	}
	MFVarSetFloat (_MDOutCommon_GrossRadID,  itemID, grossRad / 24.0);
}

enum { MDhelp, MDinput, MDstandard,  MDOtto }; 

int MDCommon_GrossRadDef () {
	int optID = MDinput;
	const char *optStr;
	const char *options [] = { MFhelpStr, MFinputStr, "standard", "Otto", (char *) NULL };
	float par;

	if (_MDOutCommon_GrossRadID != MFUnset) return (_MDOutCommon_GrossRadID);

	MFDefEntering ("Gross Radiance");
	if ((optStr = MFOptionGet (MDVarCore_GrossRadiance)) != (char *) NULL) optID = CMoptLookup (options, optStr, true);
	switch (optID) {
		default:
		case MDhelp:  MFOptionMessage (MDVarCore_GrossRadiance, optStr, options); return (CMfailed);
		case MDinput: _MDOutCommon_GrossRadID = MFVarGetID (MDVarCore_GrossRadiance, "W/m^2", MFInput, MFFlux, MFBoundary); break;
		case MDstandard:
			if (((optStr = MFOptionGet (MDParGrossRadTAU)) != (char *) NULL) && (sscanf (optStr,"%f",&par) == 1)) _MDGrossRadStdTAU = par;
			if (((_MDOutCommon_GrossRadID    = MFVarGetID (MDVarCore_GrossRadiance, "W/m^2", MFOutput, MFFlux, MFBoundary)) == CMfailed) ||
                (MFModelAddFunction(_MDCommon_GrossRadianceStd) == CMfailed)) return (CMfailed);
			break;
		case MDOtto:
			if (((_MDOutCommon_GrossRadID    = MFVarGetID (MDVarCore_GrossRadiance, "W/m^2", MFOutput, MFFlux, MFBoundary)) == CMfailed) ||
                (MFModelAddFunction(_MDCommon_GrossRadianceOtto) == CMfailed)) return (CMfailed);
			break;
	}
	MFDefLeaving ("Gross Radiance");
	return (_MDOutCommon_GrossRadID);
}
