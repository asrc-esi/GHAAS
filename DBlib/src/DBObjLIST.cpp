/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

DBObjLIST.cpp

bfekete@gc.cuny.edu

*******************************************************************************/

#include <string.h>
#include <DB.hpp>

int _DBObjectLISTNameSort(const DBObject **obj0, const DBObject **obj1) {
    return (strcmp((*obj0)->Name(), (*obj1)->Name()));
}

int _DBObjectLISTNameReversedSort(const DBObject **obj0, const DBObject **obj1) {
    return (strcmp((*obj1)->Name(), (*obj0)->Name()));
}
