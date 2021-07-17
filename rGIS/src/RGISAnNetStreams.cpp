/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2021, UNH - ASRC/CUNY

RGISAnNetStreams.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <rgis.hpp>

void RGISAnNetworkStreamLinesCBK (Widget widget,RGISWorkspace *workspace,XmAnyCallbackStruct *callData)

	{
	char *orderField;
    DBInt minOrder = 3;
	DBDataset *dataset = UIDataset ();
	DBObjData *netData = dataset->Data ();
	DBObjTable *cellTable = netData->Table (DBrNCells);
	DBObjData *arcData = new DBObjData ("",DBTypeVectorLine);
	static Widget fieldSelect = (Widget) NULL;

	if (fieldSelect == (Widget) NULL)	fieldSelect = UISelectionCreate ((char *) "Select Field");
	if ((orderField = UISelectObject (fieldSelect,(DBObjectLIST<DBObject> *) cellTable->Fields (),DBTableFieldIsInteger)) == (char *) NULL)
		return;
	if (cellTable->Field (orderField) == (DBObjTableField *) NULL)
		{ CMmsgPrint (CMmsgAppError, "Field Selection Error in: %s %d",__FILE__,__LINE__); return; }
	arcData->Document (DBDocGeoDomain,netData->Document (DBDocGeoDomain));
	arcData->Document (DBDocSubject,"Stream Lines");
	if (UIDataHeaderForm (arcData))
		{
        UIPauseDialogOpen ((char *) "Creating Stream Lines");
        if (RGlibNetworkToStreamlines (netData, minOrder, arcData) == CMsucceeded)
            workspace->CurrentData (arcData);
        else delete arcData;
    	UIPauseDialogClose ();
		}
	}
