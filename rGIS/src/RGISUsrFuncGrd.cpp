/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

RGISUsrFuncGrd.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <Xm/Xm.h>
#include <rgis.hpp>

void _RGISUserFuncionGrid (DBObjData *data,UI2DView *view,XEvent *event)

	{
	void _RGISUserFuncionQuery (DBObjData *,UI2DView *,XEvent *);

	if ((data->Flags () & DBDataFlagUserModeFlags) == DBDataFlagUserModeQuery)
		{ _RGISUserFuncionQuery (data,view,event);	return; }
	}
