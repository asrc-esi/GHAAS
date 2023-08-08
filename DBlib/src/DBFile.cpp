/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2023, UNH - ASRC/CUNY

DBFile.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <sys/stat.h>
#include <DB.hpp>

DBInt DBFileSize(const char *fileName) {
    struct stat fileStat;

    if (stat(fileName, &fileStat) == DBFault) return (DBFault);
    return (fileStat.st_size);
}
