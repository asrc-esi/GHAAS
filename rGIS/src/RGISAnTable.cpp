/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

RGISAnTable.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <rgis.hpp>

extern void RGISAnGNUXYPlotCBK (Widget,RGISWorkspace *,XmAnyCallbackStruct *);

UIMenuItem RGISAnalyseTableMenu [] = {
	UIMenuItem ((char *) "GNU XY-Plot",				RGISTableGroup,	UIMENU_NORULE, 				(UIMenuCBK) RGISAnGNUXYPlotCBK,						(char *) NULL),
	UIMenuItem ()};
