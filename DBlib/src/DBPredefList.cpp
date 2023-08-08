/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

DBPredefList.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>

char *_DBPredefinedSubjetList[] = {
        GHAASSubjNetwork,
        GHAASSubjRunoff,
        GHAASSubjDischarge,
        GHAASSubjRiverStorage,
        GHAASSubjFlowVelocity,
        GHAASSubjPrecip,
        GHAASSubjAirTemp,
        GHAASSubjHumidity,
        GHAASSubjVaporPres,
        GHAASSubjWindSpeed,
        GHAASSubjSolarRad,
        GHAASSubjStations,
        GHAASSubjReservoirs,
        GHAASSubjElevation,
        (char *) NULL};

char *_DBPredefinedGeoDomainList[] = {
        (char *) "Africa",
        (char *) "Asia",
        (char *) "Australia",
        (char *) "Europe",
        (char *) "North America",
        (char *) "South America",
        (char *) "Central America",
        (char *) "Arctic Region",
        (char *) "Canada",
        (char *) "Hungary",
        (char *) "Russia",
        (char *) "United States",
        (char *) "Amazonas",
        (char *) "Danube",
        (char *) "Lena",
        (char *) "Mississippi",
        (char *) "Gulf of Maine",
        (char *) NULL};

