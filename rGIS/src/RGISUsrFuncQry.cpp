/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

RGISUsrFuncQry.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <Xm/Xm.h>
#include <rgis.hpp>

void _RGISUserFuncionQuery (DBObjData *dbData,UI2DView *view,XEvent *event) {
	DBInt sX, sY;
	DBCoordinate coord;
	DBObjData *data = dbData;
	UIAttribView *attribView;

	if (event->type != ButtonPress) return;
	sX = event->xbutton.x;
	sY = event->xbutton.y;
	view->Window2Map  (sX,sY, &(coord.X), &(coord.Y));

	for (data = dbData;
		  (data != (DBObjData *) NULL) && ((data->Flags () & DBObjectFlagProcessed) != DBObjectFlagProcessed);
		  data = data->LinkedData ()) {
		data->Flags (DBObjectFlagProcessed,DBSet);
		if ((data->Flags () & DBDataFlagUserModeFlags) == DBDataFlagUserModeQuery) {
			if ((attribView = (UIAttribView *) data->Display (UIAttribViewSTR)) == (UIAttribView *) NULL)
				data->DispAdd (attribView = new UIAttribView (data));
			switch (data->Type ()) {
				case DBTypeVectorPoint:
				case DBTypeVectorLine:
				case DBTypeVectorPolygon: {
					DBVectorIF *vectorIF = new DBVectorIF (data);
					attribView->Draw (vectorIF->Item (coord));
				} break;
				case DBTypeGridDiscrete: {
					DBGridIF *gridIF = new DBGridIF (data);
					attribView->Draw (gridIF->GridItem (coord));
					delete gridIF;
				} break;
				case DBTypeGridContinuous: {
					DBPosition pos;
					DBGridIF *gridIF = new DBGridIF (data);
					char nameString [DBStringLength * 2];
					DBObjRecord *record = new DBObjRecord ("Noname",sizeof (DBCoordinate));
					gridIF->Coord2Pos (coord,pos);
					gridIF->Pos2Coord (pos,coord);
					snprintf (nameString, sizeof(nameString), "CellCoordinate %10.2f%10.2f", coord.X,coord.Y);
					record->Name (nameString);
					memcpy (record->Data (),(void *) &coord,sizeof (coord));
					attribView->Draw (record);
					delete record;
					delete gridIF;
				} break;
				case DBTypeNetwork: {
					DBNetworkIF *netIF = new DBNetworkIF (data);
					attribView->Draw (netIF->Cell (coord));
					delete netIF;
				} break;
				default:
					CMmsgPrint (CMmsgAppError, "Invalid Data Type in: %s %d",__FILE__,__LINE__); break;
			}
		}
	}
	for (data = dbData; (data != (DBObjData *) NULL) && ((data->Flags () & DBObjectFlagProcessed) == DBObjectFlagProcessed); data = data->LinkedData ())
		data->Flags (DBObjectFlagProcessed,DBClear);
}
