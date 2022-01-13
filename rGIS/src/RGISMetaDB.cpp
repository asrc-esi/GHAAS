/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2022, UNH - ASRC/CUNY

RGISMetaDB.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <rgis.hpp>

static void _RGISMetaDBDeleteCBK (Widget widget,RGISWorkspace *workspace,XmAnyCallbackStruct *callData)

	{
	char *subjectSTR = strlen (workspace->Subject ()) > 0 ? workspace->Subject () : (char *) NULL;
	char *domainSTR  = strlen (workspace->Domain ())  > 0 ? workspace->Domain ()  : (char *) NULL;
	DBObjMetaEntry *metaEntry;
	DBDataset *dataset = UIDataset ();

	if ((metaEntry = UIDatasetMetaData (subjectSTR,domainSTR,DBFault)) != (DBObjMetaEntry *) NULL)
		dataset->DeleteMetaEntry (metaEntry);
	}

UIMenuItem RGISMetaDBMenu [] = {
	UIMenuItem ((char *) "Refresh",	UIMENU_NORULE,		UIMENU_NORULE,	(UIMenuCBK) UIMenuNotImplemented, (char *) "RGIS22MenuSystem.html#MetaDB_Refresh"),
	UIMenuItem ((char *) "Edit",		UIMENU_NORULE, 	UIMENU_NORULE,	(UIMenuCBK) UIMenuNotImplemented, (char *) "RGIS22MenuSystem.html#MetaDB_Edit"),
	UIMenuItem ((char *) "Delete",	UIMENU_NORULE, 	UIMENU_NORULE,	(UIMenuCBK) _RGISMetaDBDeleteCBK, (char *) "RGIS22MenuSystem.html#MetaDB_Delete"),
	UIMenuItem ()};
