/******************************************************************************

GHAAS RiverGIS Library V2.1
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

RG.H

bfekete@ccny.cuny.edu

*******************************************************************************/

#define RGlibAggrMinimum 0
#define RGlibAggrMaximum 1
#define RGlibAggrAverage 2
#define RGlibAggrSum     3

#define RGlibCycleStep(cycle, tStep) ((tStep) % cycle)

#define RGlibCellID         ((char *) "CellID")
#define RGlibBasinName      ((char *) "BasinName")
#define RGlibColor          ((char *) "Color")
#define RGlibCellNum        ((char *)  "NumberOfCells")
#define RGlibLength         ((char *) "STNMainstemLength")
#define RGlibArea           ((char *) "STNCatchmentArea")
#define RGlibInterStation   ((char *) "STNInterStationArea")
#define RGlibNextStation    ((char *) "NextStation")

#define RGlibMinSLOPE 0.001

typedef enum {RGlibSQLCaseSensitive = 0, RGlibSQLCaseLower = 1, RGlibSQLCaseUpper = 2 } RGlibSQLCase;

DBInt RGlibGenFuncFieldCompare(DBObjTable *, char *, char *, char *, DBInt);

DBInt RGlibGenFuncFieldCalculate(DBObjTable *, char *, char *, char *, DBInt);

DBInt RGlibGenFuncTopoAccum(DBObjTable *, char *, char *, char *);

DBInt RGlibGenFuncTopoSubtract(DBObjTable *, char *, char *, char *);

DBInt RGlibGenFuncSymbolField(DBObjData *, const char *);

DBInt RGlibPointSTNCoordinates(DBObjData *, DBObjTableField *, DBObjTableField *,DBFloat, DBFloat, bool);

DBInt RGlibPointSTNCharacteristics(DBObjData *);

#define RGlibMassCoordX    "MassCoordX"
#define RGlibMassCoordY    "MassCoordY"

DBInt RGlibPointSubbasinCenter(DBObjData *, DBObjData *);

DBInt RGlibPointInterStationTS(DBObjData *, DBObjData *, char *, char *);

DBInt RGlibPointSubbasinStats(DBObjData *, DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibPointSubbasinHist(DBObjData *, DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibGridNetFilter (DBObjData *, DBObjData *);

DBInt RGlibGridRemovePits(DBObjData *, DBObjData *);

DBInt RGlibGridResampling(DBObjData *, DBObjRecord *, DBObjData *, DBObjData *);

DBInt RGlibGridUniformRunoff(DBObjData *, DBObjData *, char *, char *, char *, char *, DBObjData *);

DBInt RGlibGridUniformGrid(DBObjData *, DBObjData *, char *, char *, char *, char *, DBObjData *);

DBInt RGlibGridReclassDiscrete(DBObjData *, char *, DBObjData *);

DBInt RGlibGridReclassContinuous(DBObjData *, char *, DBObjData *);

DBInt RGlibGridMakeDiscrete(DBObjData *, DBObjData *, float [], int);

DBInt RGlibGridZoneHistogram(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibGridZoneStatistics(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibGridNoNegatives(DBObjData *, DBInt);

DBInt RGlibGridCellStats(DBObjData *, DBObjData *);

DBInt RGlibGridBivarCellStats(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibGridSampling(DBObjData *, DBObjData *, DBObjData *);

void RGlibGridSampling(DBObjData *, DBObjData *, bool, bool);

void RGlibGridSampling(DBObjData *, DBObjData *, bool);

DBInt RGlibCycleMean(DBObjData *, DBObjData *, DBInt, DBInt);

DBInt RGlibTSAggregate(DBObjData *, DBObjData *, DBInt, DBInt);

DBInt RGlibSeasonAggregate(DBObjData *, DBObjData *, DBInt, DBInt, DBInt);

DBInt RGlibSeasonMean(DBObjData *, DBObjData *, DBInt, DBInt, DBInt);

DBInt RGlibMinMax(DBObjData *, DBObjData *, bool, bool);

DBInt RGlibNetworkToGrid(DBObjData *, DBObjTableField *, DBObjData *);

DBInt RGlibNetworkBasinGrid(DBObjData *, DBObjData *);

DBInt RGlibNetworkStations(DBObjData *, DBFloat, DBFloat, DBObjData *);

DBInt RGlibNetworkAccumulate(DBObjData *, DBObjData *, DBObjData *, DBObjData *, char **, DBFloat, bool, bool, bool,
                             DBObjData *);

DBInt RGlibNetworkAccumulate(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkUnaccumulate(DBObjData *, DBObjData *, DBFloat, bool, DBObjData *);

DBInt RGlibNetworkInvAccumulate(DBObjData *, DBObjData *, DBObjData *, DBFloat);

DBInt RGlibNetworkBasinDistrib(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkBasinProf(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkBasinStats(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkCellSlopes(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkConfluences(DBObjData *, DBObjData *);

DBInt RGlibNetworkUpStreamAvg(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkHeadStats(DBObjData *, DBObjData *, DBObjData *, DBInt);

DBInt RGlibNetworkHistogram(DBObjData *, DBObjData *, DBObjData *);

DBInt RGlibNetworkPourElevation (DBObjData *, const char *, const char *);

DBInt RGlibNetworkReservoir (DBObjData *, const char *, const char *, const char *, const char *);

DBInt RGlibNetworkDefragment(DBObjData *, DBObjData *, DBFloat, DBFloat, bool);

DBInt RGlibNetworkToStreamlines (DBObjData *, DBInt, DBObjData *);

DBInt RGLibTableJoin(DBObjTable *, DBObjTableField *, DBObjTable *, DBObjTableField *);

typedef enum {RGlibTableCopy, RGlibTableAppend, RGlibTableReplace, RGlibTableBlank } RGlibTableAction;

typedef enum {RGlibSQLpostgres, RGlibSQLite} RGlibSQLdialect;

DBInt RGlibTableToSQL (DBObjTable *, const char *, const char *, RGlibTableAction, bool, RGlibSQLdialect, DBInt, FILE *);

DBInt RGlibRGIS2DataStream(DBObjData *, DBObjData *, FILE *, CMthreadTeam_p);

DBInt RGlibDataStream2RGIS(DBObjData *, DBObjData *, FILE *);

void RGlibPauseOpen(const char *);

void RGlibPauseClose();
