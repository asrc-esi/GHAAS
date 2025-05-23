/******************************************************************************

GHAAS RiverGIS V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

RGISToolsGrdExp.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <ctype.h>
#include <rgis.hpp>

int _RGISToolsGridExportARCInfo (DBObjData *data,char *selection)

	{
	DBInt layerID, i;
	FILE *file;
	char coverName	[11], command    [256]; 
	char asciiGrid   		[FILENAME_MAX];
	char attribDef   		[FILENAME_MAX];
	char attribData   	[FILENAME_MAX];
	char amlFile    [FILENAME_MAX];
	DBGridIF *gridIF = new DBGridIF (data);
	DBObjRecord *layerRec;

	for (layerID = 0;layerID < gridIF->LayerNum ();++layerID)
		{
		layerRec = gridIF->Layer (layerID);
		snprintf (asciiGrid, sizeof(asciiGrid), "%s/asciigrid%d.tmp",selection,layerID);
		if (DBExportARCGridLayer (data,layerRec,asciiGrid) == DBFault) return (DBFault);
		}
	if (data->Type () == DBTypeGridDiscrete)
		{
		snprintf (attribDef, sizeof(attribDef), "%s/attrib.def",selection);
		if (DBExportARCTableDef (data,DBrNItems,attribDef) == DBFault)
			{ unlink (asciiGrid); return (DBFault); }
		snprintf (attribData,sizeof(attribData), "%s/attrib.dat",selection);
		if (DBExportARCTableData (data,DBrNItems,attribData) == DBFault)
			{ unlink (asciiGrid); unlink (attribDef); return (DBFault); }
		}
	
	snprintf (amlFile, sizeof(amlFile), "%s/grdcreate.aml",selection);
	if ((file = fopen (amlFile,"w")) == NULL)
		{
		for (layerID = 0;layerID < gridIF->LayerNum ();++layerID)
			{
			layerRec = gridIF->Layer (layerID);
			snprintf (asciiGrid, sizeof(asciiGrid), "%s/asciigrid%d.tmp",selection,layerID);
			unlink (asciiGrid);
			}
		if (data->Type () == DBTypeGridDiscrete) { unlink (attribDef); unlink (attribData); }
		CMmsgPrint (CMmsgAppError, "Aml File Creation Error in: %s %d",__FILE__,__LINE__);
		return (DBFault);
		}
	fprintf (file,"&workspace %s\n",selection);
	for (layerID = 0;layerID < gridIF->LayerNum ();++layerID)
		{
		layerRec = gridIF->Layer (layerID);
		strncpy (coverName,layerRec->Name (),sizeof (coverName) - 1);
		coverName	[sizeof (coverName) - 1] = '\0';
		for (i = 0;i < (DBInt) strlen (coverName);++i) if (coverName [i] == '.') coverName [i] = '\0';
		for (i = 0;i < (DBInt) strlen (coverName);++i) coverName [i] = tolower (coverName [i]);
		snprintf (asciiGrid, sizeof(asciiGrid), "%s/asciigrid%d.tmp",selection,layerID);
		if (data->Type () == DBTypeGridContinuous)
			fprintf (file,"asciigrid %s g_%s float\n",asciiGrid,coverName);
		else
			fprintf (file,"asciigrid %s g_%s int\n",asciiGrid,coverName);
		if (data->Type () == DBTypeGridDiscrete)
			{
			fprintf (file,"tables\n");
			fprintf (file,"define g_%s.vattr\n",coverName);
			fprintf (file,"&r %s\n",attribDef);
			fprintf (file,"~\n");
			fprintf (file,"alter GridValue\n");
			fprintf (file,"value,,,,\n");
			fprintf (file,"add\n");
			fprintf (file,"&r %s\n",attribData);
			fprintf (file,"~\n");
			fprintf (file,"q stop\n");
			fprintf (file,"joinitem g_%s.vat g_%s.vattr g_%s.vat value count\n",coverName,coverName,coverName);
			fprintf (file,"tables\n");
			fprintf (file,"select g_%s.vattr\n",coverName);
			fprintf (file,"erase g_%s.vattr\ny\n",coverName);
			fprintf (file,"q stop\n");
			}
		}
	fclose (file);

	if (getenv ("GHAAS_ARC") != NULL)
		{
		snprintf (command, sizeof(command), getenv ("GHAAS_ARC"),amlFile);

		system (command);
		for (layerID = 0;layerID < gridIF->LayerNum ();++layerID)
			{
			layerRec = gridIF->Layer (layerID);
			snprintf (asciiGrid, sizeof(asciiGrid), "%s/asciigrid%d.tmp",selection,layerID);
			unlink (asciiGrid);
			}
		if (data->Type () == DBTypeGridDiscrete) { unlink (attribDef); unlink (attribData); }
		unlink (amlFile);
		}
	return (DBSuccess);
	}
	
