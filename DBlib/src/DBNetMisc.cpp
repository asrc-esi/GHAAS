/******************************************************************************

GHAAS Database library V3.0
Global Hydrological Archive and Analysis System
Copyright 1994-2024, UNH - CCNY

DBNetMisc.cpp

bfekete@ccny.cuny.edu

*******************************************************************************/

#include <DB.hpp>
#include <DBif.hpp>

#define _DBNetworkOppositeDirection(dir) (((dir >> 0x04) | (dir << 0x04)) & 0xff)

DBNetworkIF::DBNetworkIF(DBObjData *data) {
    DBObjTableField *layerFLD;

    DataPTR = data;
    BasinTable = data->Table(DBrNItems);
    CellTable = data->Table(DBrNCells);
    LayerTable = data->Table(DBrNLayers);
    SymbolTable = data->Table(DBrNSymbols);

    MouthPosFLD = BasinTable->Field(DBrNMouthPos);
    ColorFLD = BasinTable->Field(DBrNColor);
    BasinOrderFLD = BasinTable->Field(DBrNBasinOrder);
    SymbolFLD = BasinTable->Field(DBrNSymbol);
    BasinLengthFLD = BasinTable->Field(DBrNBasinLength);
    BasinAreaFLD = BasinTable->Field(DBrNBasinArea);

    SymbolIDFLD = SymbolTable->Field(DBrNSymbolID);
    ForegroundFLD = SymbolTable->Field(DBrNForeground);
    BackgroundFLD = SymbolTable->Field(DBrNBackground);

    PositionFLD = CellTable->Field(DBrNPosition);
    ToCellFLD = CellTable->Field(DBrNToCell);
    FromCellFLD = CellTable->Field(DBrNFromCell);
    OrderFLD = CellTable->Field(DBrNOrder);
    MagnitudeFLD = CellTable->Field(DBrNMagnitude);
    BasinFLD = CellTable->Field(DBrNBasin);
    BasinCellsFLD = CellTable->Field(DBrNBasinCells);
    TravelFLD = CellTable->Field(DBrNTravel);
    UpCellPosFLD = CellTable->Field(DBrNUpCellPos);
    CellAreaFLD = CellTable->Field(DBrNCellArea);
    CellLengthFLD = CellTable->Field(DBrNCellLength);
    DistToMouthFLD = CellTable->Field(DBrNDistToMouth);
    DistToOceanFLD = CellTable->Field(DBrNDistToOcean);
    SubbasinLengthFLD = CellTable->Field(DBrNSubbasinLength);
    SubbasinAreaFLD = CellTable->Field(DBrNSubbasinArea);

    RowNumFLD = LayerTable->Field(DBrNRowNum);
    ColNumFLD = LayerTable->Field(DBrNColNum);
    CellWidthFLD = LayerTable->Field(DBrNCellWidth);
    CellHeightFLD = LayerTable->Field(DBrNCellHeight);
    layerFLD = LayerTable->Field(DBrNLayer);

    if ((LayerRecord = LayerTable->Item(DBrNLookupGrid)) == (DBObjRecord *) NULL) {
        CMmsgPrint(CMmsgAppError, "Bailing Out like ARC/INFO in: %s %d", __FILE__, __LINE__);
        exit(-1);
    }
    if ((DataRec = layerFLD->Record(LayerRecord)) == (DBObjRecord *) NULL) {
        CMmsgPrint(CMmsgAppError, "Bailing Out in: %s %d", __FILE__, __LINE__);
        exit(-1);
    }
    if (CellLengthFLD == (DBObjTableField *) NULL) {
        DBInt cellID;
        DBObjRecord *cellRec;
        DBCoordinate coord0, coord1;

        CellLengthFLD = new DBObjTableField(DBrNCellLength, DBTableFieldFloat, "%10.1f", sizeof(DBFloat4), true);
        CellTable->AddField(CellLengthFLD);
        for (cellID = 0; cellID < CellNum(); ++cellID) {
            cellRec = Cell(cellID);
            coord0 = Center(cellRec);
            coord1 = Center(cellRec) + Delta(cellRec);
            CellLengthFLD->Float(cellRec, DBMathCoordinateDistance(DataPTR->Projection(), coord0, coord1));
        }
    }
}

DBInt DBNetworkIF::Coord2Pos(DBCoordinate coord, DBPosition &pos) const {
    DBInt ret = DataPTR->Extent().InRegion(coord) ? DBSuccess : DBFault;

    pos.Col = (DBUShort) floor((coord.X - DataPTR->Extent().LowerLeft.X) / CellWidth());
    pos.Row = (DBUShort) floor((coord.Y - DataPTR->Extent().LowerLeft.Y) / CellHeight());
    return (ret);
}

DBInt DBNetworkIF::Pos2Coord(DBPosition pos, DBCoordinate &coord) const {
    DBInt ret = DBSuccess;
    if (pos.Col < 0) ret = DBFault;
    if (pos.Row < 0) ret = DBFault;
    if (pos.Col >= ColNum()) ret = DBFault;
    if (pos.Row >= RowNum()) ret = DBFault;

    coord.X = DataPTR->Extent().LowerLeft.X + pos.Col * CellWidth() + CellWidth() / 2.0;
    coord.Y = DataPTR->Extent().LowerLeft.Y + pos.Row * CellHeight() + CellHeight() / 2.0;
    return (ret);
}

DBCoordinate DBNetworkIF::Center(const DBObjRecord *cellRec) const {
    DBCoordinate coord;
    DBPosition cellPos = PositionFLD->Position(cellRec);

    coord.X = DataPTR->Extent().LowerLeft.X + cellPos.Col * CellWidth() + CellWidth() / 2.0;
    coord.Y = DataPTR->Extent().LowerLeft.Y + cellPos.Row * CellHeight() + CellHeight() / 2.0;
    return (coord);
}

DBCoordinate DBNetworkIF::Delta(const DBObjRecord *cellRec) const {
    DBInt toCell = ToCellFLD->Int(cellRec);
    DBCoordinate delta(0.0, 0.0);

    if ((toCell == DBNetDirNW) || (toCell == DBNetDirN) || (toCell == DBNetDirNE)) delta.Y = CellHeight();
    if ((toCell == DBNetDirSE) || (toCell == DBNetDirS) || (toCell == DBNetDirSW)) delta.Y = -CellHeight();
    if ((toCell == DBNetDirNE) || (toCell == DBNetDirE) || (toCell == DBNetDirSE)) delta.X = CellWidth();
    if ((toCell == DBNetDirNW) || (toCell == DBNetDirW) || (toCell == DBNetDirSW)) delta.X = -CellWidth();
    return (delta);
}

DBObjRecord *DBNetworkIF::Cell(DBPosition pos) const {
    DBInt cellID;
    if (pos.Col < 0) return ((DBObjRecord *) NULL);
    if (pos.Row < 0) return ((DBObjRecord *) NULL);
    if (pos.Col >= ColNum()) return ((DBObjRecord *) NULL);
    if (pos.Row >= RowNum()) return ((DBObjRecord *) NULL);

    if ((cellID = ((DBInt *) DataRec->Data())[pos.Row * ColNum() + pos.Col]) == DBFault) return ((DBObjRecord *) NULL);
    return (CellTable->Item(cellID));
}

DBObjRecord *DBNetworkIF::Cell(DBPosition pos, DBObjTableField *field, DBFloat target, DBInt pRadius, DBFloat tolerance) const {
    DBInt i, j, k, l, radius, dir, pR2 = pRadius * pRadius;
    DBInt cellID;
    DBFloat bestDelta, delta, val;
    DBObjRecord *cellRec, *bestCellRec = (DBObjRecord *) NULL;
    DBPosition cellPos;

    if (pos.Col < 0)         return ((DBObjRecord *) NULL);
    if (pos.Row < 0)         return ((DBObjRecord *) NULL);
    if (pos.Col >= ColNum()) return ((DBObjRecord *) NULL);
    if (pos.Row >= RowNum()) return ((DBObjRecord *) NULL);

    if (tolerance <= 0.0) { // When tolerance is zero or negative searching for largest value irrespective ot the target value
        bestDelta = -HUGE_VAL;
        for (radius = 0; radius < pRadius; ++radius) for (l = 0;l < 4; ++l)  for (k = 0; k <= radius; ++k) for (dir = 0; dir < 2; ++dir) {
            switch (l) {
                case 0: i = dir == 0 ? k : -k; j = - radius;          break; // Horizontal line North
                case 1: i = radius;            j = dir == 0 ? k : -k; break; // Vertical line East
                case 2: i = dir == 0 ? k : -k; j =   radius;          break; // Horizon line South
                case 3: i = - radius;          j = dir == 0 ? k : -k; break; // Vertical line West
                default: continue;
            }
            if ((dir == 1) && (i == 0) && (j == 0)) continue;
            if (i * i + j * j > pR2)                continue;
            
            cellPos.Col = pos.Col + i;
            cellPos.Row = pos.Row + j;
            if (cellPos.Col < 0)         continue;
            if (cellPos.Row < 0)         continue;
            if (cellPos.Col >= ColNum()) continue;
            if (cellPos.Row >= RowNum()) continue;

            if ((cellID = ((DBInt *) DataRec->Data())[(size_t) cellPos.Row * (size_t) ColNum() + (size_t) cellPos.Col]) == DBFault) continue;
            cellRec = CellTable->Item(cellID);
            val     = field->Float(cellRec);
            if (!CMmathEqualValues(val, field->FloatNoData()) && (val > 0.0)) {
                if (val > bestDelta) {
                    bestDelta   = val;
                    bestCellRec = cellRec;
                }
            }
        }
    } else {
        bestDelta = HUGE_VAL;
        for (radius = 0; radius < pRadius; ++radius) for (l = 0;l < 4; ++l)  for (k = 0; k <= radius; ++k) for (dir = 0; dir < 2; ++dir) {
            switch (l) {
                case 0: i = dir == 0 ? k : -k; j = - radius;          break; // Horizontal line North
                case 1: i = radius;            j = dir == 0 ? k : -k; break; // Vertical line East
                case 2: i = dir == 0 ? k : -k; j =   radius;          break; // Horizon line South
                case 3: i = - radius;          j = dir == 0 ? k : -k; break; // Vertical line West
                default: continue;
            }
            if ((dir == 1) && (i == 0) && (j == 0)) continue;
            if (i * i + j * j > pR2)                continue;
            
            cellPos.Col = pos.Col + i;
            cellPos.Row = pos.Row + j;
            if (cellPos.Col < 0)         continue;
            if (cellPos.Row < 0)         continue;
            if (cellPos.Col >= ColNum()) continue;
            if (cellPos.Row >= RowNum()) continue;

            if ((cellID = ((DBInt *) DataRec->Data())[(size_t) cellPos.Row * (size_t) ColNum() + (size_t) cellPos.Col]) == DBFault) continue;
            cellRec = CellTable->Item(cellID);
            val     = field->Float(cellRec);
            if (CMmathEqualValues(val, field->FloatNoData())) continue;
            delta = fabs(val - target);
            if (delta < bestDelta) {
                bestDelta   = delta;
                bestCellRec = cellRec;
            }
        if (fabs(bestDelta) / (val + target) < tolerance) break;
        }
    }
    return (bestCellRec);
}

DBObjRecord *DBNetworkIF::ToCell(const DBObjRecord *cellRec) const {
    DBInt toCell;
    DBPosition pos;

    if (cellRec == (DBObjRecord *) NULL) return ((DBObjRecord *) NULL);
    toCell = ToCellFLD->Int(cellRec);
    pos = PositionFLD->Position(cellRec);

    if (toCell == 0x0L) return ((DBObjRecord *) NULL);
    if ((toCell == DBNetDirNW) || (toCell == DBNetDirN) || (toCell == DBNetDirNE)) pos.Row++;
    if ((toCell == DBNetDirSE) || (toCell == DBNetDirS) || (toCell == DBNetDirSW)) pos.Row--;
    if ((toCell == DBNetDirNE) || (toCell == DBNetDirE) || (toCell == DBNetDirSE)) pos.Col++;
    if ((toCell == DBNetDirNW) || (toCell == DBNetDirW) || (toCell == DBNetDirSW)) pos.Col--;
    return (Cell(pos));
}

DBObjRecord *DBNetworkIF::FromCell(const DBObjRecord *cellRec, DBInt dir, DBInt sameBasin) const {
    DBInt fromCell;
    DBPosition pos;

    if (cellRec == (DBObjRecord *) NULL) return ((DBObjRecord *) NULL);
    if (sameBasin) {
        fromCell = FromCellFLD->Int(cellRec);
        if ((fromCell & dir) != dir) return ((DBObjRecord *) NULL);
    }
    pos = PositionFLD->Position(cellRec);
    if ((dir == DBNetDirNW) || (dir == DBNetDirN) || (dir == DBNetDirNE)) pos.Row++;
    if ((dir == DBNetDirSE) || (dir == DBNetDirS) || (dir == DBNetDirSW)) pos.Row--;
    if ((dir == DBNetDirNE) || (dir == DBNetDirE) || (dir == DBNetDirSE)) pos.Col++;
    if ((dir == DBNetDirNW) || (dir == DBNetDirW) || (dir == DBNetDirSW)) pos.Col--;
    return (Cell(pos));
}

DBObjRecord *DBNetworkIF::FromCell(const DBObjRecord *cellRec) const {
    DBInt dir, maxDir = DBFault;
    DBObjRecord *fromCell;
    DBFloat area, maxArea = -DBHugeVal;

    for (dir = 0; dir < 8; ++dir)
        if ((fromCell = FromCell(cellRec, 0x01 << dir)) != (DBObjRecord *) NULL) {
            area = CellBasinArea(fromCell);
            if (maxArea < area) {
                maxArea = area;
                maxDir = dir;
            }
        }
    return (maxDir == DBFault ? (DBObjRecord *) NULL : FromCell(cellRec, 0x01 << maxDir));
}

DBObjRecord *DBNetworkIF::HeadCell(const DBObjRecord *cellRec) const {
    DBObjRecord *headCell;

    for (headCell = FromCell(cellRec); FromCell(headCell) != (DBObjRecord *) NULL; headCell = FromCell(headCell));
    return (headCell);
}

void DBNetworkIF::UpStreamSearch(DBObjRecord *cellRec, DBNetworkACTION forAction, DBNetworkACTION backAction,
                                 void *data) {
    DBInt dir;
    DBObjRecord *fromCell;

    if ((forAction != (DBNetworkACTION) NULL) && ((*forAction)(this, cellRec, data) == true)) {
        for (dir = 0; dir < 8; ++dir)
            if ((fromCell = FromCell(cellRec, 0x01 << dir)) != (DBObjRecord *) NULL)
                UpStreamSearch(fromCell, forAction, backAction, data);
    }
    if (backAction != (DBNetworkACTION) NULL) (*backAction)(this, cellRec, data);
}

void DBNetworkIF::DownStreamSearch(DBObjRecord *cellRec, DBNetworkACTION forAction, DBNetworkACTION backAction,
                                   void *data) {
    DBObjRecord *toCell;

    if ((forAction != (DBNetworkACTION) NULL) && ((*forAction)(this, cellRec, data) == true)) if (
            (toCell = ToCell(cellRec)) != (DBObjRecord *) NULL)
        DownStreamSearch(toCell, forAction, backAction, data);
    if (backAction != (DBNetworkACTION) NULL) (*backAction)(this, cellRec, data);
}

bool DBNetworkSelect(DBNetworkIF *netIF, DBObjRecord *cellRec, void *dataPtr) {
    if (cellRec == (DBObjRecord *) NULL) return (false);
    cellRec->Flags(DBObjectFlagSelected, DBSet);
    netIF = (DBNetworkIF *) NULL;
    return (true);
}

bool DBNetworkUnselect(DBNetworkIF *netIF, DBObjRecord *cellRec, void *dataPtr) {
    if (cellRec == (DBObjRecord *) NULL) return (false);
    cellRec->Flags(DBObjectFlagSelected, DBClear);
    netIF = (DBNetworkIF *) NULL;
    return (true);
}
DBObjRecord *DBNetworkIF::CellAdd(DBObjRecord *cellRec) {
    DBPosition pos;

    pos = CellPosition(cellRec);
    if (pos.Col < 0) return ((DBObjRecord *) NULL);
    if (pos.Row < 0) return ((DBObjRecord *) NULL);
    if (pos.Col >= ColNum()) return ((DBObjRecord *) NULL);
    if (pos.Row >= RowNum()) return ((DBObjRecord *) NULL);

    ((DBInt *) DataRec->Data())[(size_t) pos.Row * ColNum() + (size_t) pos.Col] = cellRec->RowID ();
    return (cellRec);
}

DBObjRecord *DBNetworkIF::CellAdd(DBPosition pos) {
    char nameSTR[DBStringLength];
    DBInt dir, fromDir;
    DBPosition fromPos;
    DBObjRecord *cellRec;

    if (pos.Col < 0) return ((DBObjRecord *) NULL);
    if (pos.Row < 0) return ((DBObjRecord *) NULL);
    if (pos.Col >= ColNum()) return ((DBObjRecord *) NULL);
    if (pos.Row >= RowNum()) return ((DBObjRecord *) NULL);

    if (((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + pos.Col] != DBFault) return ((DBObjRecord *) NULL);

    snprintf(nameSTR, sizeof(nameSTR), "Cell:%6d", CellNum());
    cellRec = CellTable->Add(nameSTR);
    ((DBInt *) DataRec->Data())[(size_t) pos.Row * ColNum() + (size_t) pos.Col] = cellRec->RowID();
    PositionFLD->Position(cellRec, pos);
    ToCellFLD->Int(cellRec, DBNetDirN);
    fromDir = DBNull;
    for (dir = 0; dir < 8; ++dir) {
        fromPos = pos;
        if ((dir == DBNetDirNW) || (dir == DBNetDirN) || (dir == DBNetDirNE)) fromPos.Row++;
        if ((dir == DBNetDirSE) || (dir == DBNetDirS) || (dir == DBNetDirSW)) fromPos.Row--;
        if ((dir == DBNetDirNE) || (dir == DBNetDirE) || (dir == DBNetDirSE)) fromPos.Col++;
        if ((dir == DBNetDirNW) || (dir == DBNetDirW) || (dir == DBNetDirSW)) fromPos.Col--;
        if (((DBInt *) DataRec->Data())[(size_t) fromPos.Row * (size_t) ColNum () + (size_t)  fromPos.Col] != DBFault) fromDir |= dir;
    }
    FromCellFLD->Int(cellRec, fromDir);
    OrderFLD->Int(cellRec, 1);
    BasinFLD->Int(cellRec, 1);
    BasinCellsFLD->Int(cellRec, 1);
    TravelFLD->Int(cellRec, 1);
    UpCellPosFLD->Position(cellRec, pos);
    CellAreaFLD->Float(cellRec, 0.0);
    SubbasinLengthFLD->Float(cellRec, 0.0);
    SubbasinAreaFLD->Float(cellRec, 0.0);
    return (cellRec);
}

DBInt DBNetworkIF::CellRotate(DBObjRecord *cellRec, DBInt dir) {
    DBInt toDir;

    if (cellRec == (DBObjRecord *) NULL) return (DBFault);

    toDir = CellDirection(cellRec);
    if (toDir == DBNull) {
        toDir = DBNetDirN;
        return (DBSuccess);
    }
    if (dir) toDir = toDir == DBNetDirNE ? DBNetDirE : toDir << 0x01;
    else toDir = toDir == DBNetDirE ? DBNetDirNE : toDir >> 0x01;
    CellDirection(cellRec, toDir);
    BasinFLD->Int(cellRec, DBFault);
    return (DBSuccess);
}

DBInt DBNetworkIF::CellDelete(DBObjRecord *cellRec) {
    DBPosition pos;

    if (cellRec == (DBObjRecord *) NULL) return (DBFault);
    pos = PositionFLD->Position(cellRec);
    ((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] = DBFault;
    return (DBSuccess);
}

void DBNetworkIF::Climb(DBObjRecord *cellRec, DBInt level) {
    DBInt dir, maxCell = 0, maxDir = 0, cells, maxOrder = 1, orderNo = 0;
    DBFloat maxLength = -DBHugeVal;
    DBObjRecord *fromCell;

    TravelFLD->Int(cellRec, level);

    for (dir = 0; dir < 8; ++dir)
        if ((fromCell = FromCell(cellRec, 0x01 << dir)) != (DBObjRecord *) NULL) {
            Climb(fromCell, level + 1);
            cells = BasinCellsFLD->Int(fromCell);
            BasinCellsFLD->Int(cellRec, BasinCellsFLD->Int(cellRec) + cells);
            SubbasinAreaFLD->Float(cellRec, SubbasinAreaFLD->Float(cellRec) + SubbasinAreaFLD->Float(fromCell));
            if (cells > maxCell) {
                maxCell = cells;
                maxDir = 0x01 << dir;
            }
            else if ((cells == maxCell) && (maxLength < SubbasinLengthFLD->Float(fromCell))) {
                maxLength = SubbasinLengthFLD->Float(fromCell);
                maxDir = 0x01 << dir;
            }
            if (OrderFLD->Int(fromCell) == maxOrder) {
                orderNo++;
                continue;
            }
            if (OrderFLD->Int(fromCell) > maxOrder) {
                maxOrder = OrderFLD->Int(fromCell);
                orderNo = 1;
            }
        }
    OrderFLD->Int(cellRec, maxOrder + (orderNo > 1 ? 1 : 0));

    if (maxDir != 0) {
        fromCell = FromCell(cellRec, maxDir);
        SubbasinLengthFLD->Float(cellRec, SubbasinLengthFLD->Float(cellRec) + SubbasinLengthFLD->Float(fromCell));
        UpCellPosFLD->Position(cellRec, UpCellPosFLD->Position(fromCell));
    }
}

void DBNetworkIF::SetBasin(DBObjRecord *cellRec, DBInt basin) {
    DBInt dir;
    DBObjRecord *fromCell;

    if (BasinFLD->Int(cellRec) != DBFault) return;

    BasinFLD->Int(cellRec, basin);
    for (dir = 0; dir < 8; ++dir)
        if ((fromCell = FromCell(cellRec, 0x01 << dir)) != (DBObjRecord *) NULL)
            SetBasin(fromCell, basin);
}

static DBNetworkIF *_DBnetIF;

static int _DBGNetworkCellCompare(const DBObjRecord **cellRec0, const DBObjRecord **cellRec1) {
    int ret;
    DBPosition pos0, pos1;

    if ((ret = _DBnetIF->CellBasinID(*cellRec0) - _DBnetIF->CellBasinID(*cellRec1)) != 0) return (ret);
    if (_DBnetIF->CellBasinArea(*cellRec1) != _DBnetIF->CellBasinArea(*cellRec0))
        return (_DBnetIF->CellBasinArea(*cellRec1) > _DBnetIF->CellBasinArea(*cellRec0) ? 1 : -1);
    if (_DBnetIF->CellBasinLength(*cellRec1) != _DBnetIF->CellBasinLength(*cellRec0))
        return (_DBnetIF->CellBasinLength(*cellRec1) > _DBnetIF->CellBasinLength(*cellRec0) ? 1 : -1);
    pos0 = _DBnetIF->CellPosition(*cellRec0);
    pos1 = _DBnetIF->CellPosition(*cellRec1);
    if ((ret = pos1.Row - pos0.Row) != 0) return (ret);
    return (pos1.Col - pos0.Col);
}

DBInt DBNetworkIF::Build() {
    bool changed;
    DBInt i, j, row, col, basin, dir, projection = DataPTR->Projection();
    DBCoordinate coord0, coord1;
    char nameStr[DBStringLength];
    DBPosition pos;
    DBObjRecord *cellRec, *toCell, *fromCell, *basinRec, *symbolRec;

    _DBnetIF = this;

    Rebuild:
    for (j = 0; j < BasinTable->ItemNum(); ++j) {
        basinRec = BasinTable->Item(j);
        if (((cellRec = Cell(MouthPosFLD->Position(basinRec))) == (DBObjRecord *) NULL) ||
            (ToCell(cellRec) != (DBObjRecord *) NULL)) {
            BasinTable->Delete(basinRec);
            j--;
        }
    }
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        DBPause(i * 10 / CellNum());
        if (Cell(CellPosition(cellRec))  == (DBObjRecord *) NULL) continue;
        else if (ToCell(ToCell(cellRec)) == cellRec) ToCellFLD->Int(cellRec, DBNull);
    }
    DBPause(10);

    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        DBPause(10 + i * 10 / CellNum());
        pos = CellPosition(cellRec);
        if (((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] == DBFault) {
            CellTable->Delete(cellRec);
            --i;
        }
        else {
            cellRec->Flags(DBObjectFlagLocked, DBClear);
            FromCellFLD->Int(cellRec, DBNull);
            OrderFLD->Int(cellRec, 1);
            BasinFLD->Int(cellRec, DBFault);
            BasinCellsFLD->Int(cellRec, 1);
            TravelFLD->Int(cellRec, 0);
            UpCellPosFLD->Position(cellRec, pos);
            CellAreaFLD->Float(cellRec, DBMathRectangleArea(projection, Center(cellRec) - CellSize() / 2,
                                                            Center(cellRec) + CellSize() / 2));
            coord0 = Center(cellRec);
            coord1 = Center(cellRec) + Delta(cellRec);
            CellLengthFLD->Float(cellRec, DBMathCoordinateDistance(DataPTR->Projection(), coord0, coord1));
            SubbasinLengthFLD->Float(cellRec, CellLength(cellRec));
            SubbasinAreaFLD->Float(cellRec, CellAreaFLD->Float(cellRec));
        }
    }
    for (j = 0; j < BasinTable->ItemNum(); ++j) {
        basinRec = BasinTable->Item(j);
        cellRec = Cell(MouthPosFLD->Position(basinRec));
        if ((cellRec == (DBObjRecord *) NULL) ||
            ((cellRec->Flags() & DBObjectFlagLocked) == DBObjectFlagLocked)) {
            BasinTable->Delete(basinRec);
            j--;
        }
        else cellRec->Flags(DBObjectFlagLocked, DBSet);
    }
    DBPause(20);

    for (row = 0; row < RowNum(); row++)
        for (col = 0; col < ColNum(); col++)
            ((DBInt *) DataRec->Data())[(size_t) row * (size_t) ColNum() + (size_t) col] = DBFault;
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        cellRec->Flags(DBObjectFlagLocked, DBClear);
        DBPause(20 + i * 10 / CellNum());
        pos = CellPosition(cellRec);
        if (((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] != DBFault) {
            CellTable->Delete(cellRec);
            --i;
        }
        else
            ((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] = cellRec->RowID();
    }
    DBPause(30);

    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        if ((toCell = ToCell(cellRec)) != (DBObjRecord *) NULL)
            FromCellFLD->Int(toCell, FromCellFLD->Int(toCell) | _DBNetworkOppositeDirection (ToCellFLD->Int(cellRec)));
    }
    for (i = 0; i < CellNum(); ++i) {
        DBPause(30 + i * 10 / CellNum());
        cellRec = CellTable->Item(i);
        if (ToCell(cellRec) == (DBObjRecord *) NULL) Climb(cellRec, 0);
    }
    DBPause(40);
    CellTable->ItemSort(_DBGNetworkCellCompare);
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        DBPause(35 + i * 10 / CellNum());
        pos = CellPosition(cellRec);
        ((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] = cellRec->RowID();
    }
    DBPause(50);
    basin = 0;
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        DBPause(50 + i * 20 / CellNum());
        if (ToCell(cellRec) == (DBObjRecord *) NULL) {
            SetBasin(cellRec, basin + 1);
            basinRec = (DBObjRecord *) NULL;
            for (j = 0; j < BasinTable->ItemNum(); ++j) {
                if (((basin - j) >= 0) && ((basin - j) < BasinTable->ItemNum()) &&
                    (cellRec == MouthCell(basinRec = BasinTable->Item(basin - j))))
                    break;
                if (((basin + j) < BasinTable->ItemNum()) &&
                    (cellRec == MouthCell(basinRec = BasinTable->Item(basin + j))))
                    break;
                basinRec = (DBObjRecord *) NULL;
            }
            if (basinRec == (DBObjRecord *) NULL) {
                basinRec = BasinTable->Add("GHAASBasin");
                MouthPosFLD->Position(basinRec, CellPosition(cellRec));
            }
            basinRec->ListPos(basin++);
        }
    }
    DBPause(70);
    CellTable->ItemSort(_DBGNetworkCellCompare);
    BasinTable->ItemSort();
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        DBPause(70 + i * 10 / CellNum());
        pos = CellPosition(cellRec);
        ((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] = cellRec->RowID();
        snprintf(nameStr, sizeof(nameStr), "GHAASCell:%d", cellRec->RowID() + 1);
        cellRec->Name(nameStr);
    }
    DBPause(80);
    SymbolTable->DeleteAll();
    symbolRec = SymbolTable->Add("Network Symbol");
    SymbolIDFLD->Int(symbolRec, 1);
    ForegroundFLD->Int(symbolRec, 1);
    BackgroundFLD->Int(symbolRec, 0);

    for (j = 0; j < BasinTable->ItemNum(); ++j) {
        basinRec = BasinTable->Item(j);
        if ((strncmp(basinRec->Name(), "GHAASBasin", strlen("GHAASBasin")) == 0) ||
            (strlen(basinRec->Name()) < 1)) {
            snprintf(nameStr, sizeof(nameStr), "GHAASBasin%d", basinRec->RowID() + 1);
            basinRec->Name(nameStr);
        }
        ColorFLD->Int(basinRec, DBFault);
        BasinOrderFLD->Int(basinRec, CellOrder(MouthCell(basinRec)));
        BasinAreaFLD->Float(basinRec, CellBasinArea(MouthCell(basinRec)));
        BasinLengthFLD->Float(basinRec, CellBasinLength(MouthCell(basinRec)));
        SymbolFLD->Record(basinRec, symbolRec);
    }
    basin = DBFault;
    changed = false;
    for (i = 0; i < CellNum (); ++i) {
        cellRec = CellTable->Item(i);
        if ((Basin(cellRec)) == (DBObjRecord *) NULL) {
            fromCell = cellRec;
            for (toCell = ToCell(cellRec);
                 (toCell != (DBObjRecord *) NULL) && ((toCell->Flags() & DBObjectFlagLocked) != DBObjectFlagLocked);
                 toCell = ToCell(fromCell)) {
                fromCell = toCell;
                fromCell->Flags(DBObjectFlagLocked, DBSet);
            }
            if (toCell != (DBObjRecord *) NULL) {
                CellDirection(fromCell, 0);
                changed = true;
            }
            for (toCell = ToCell(cellRec);
                 (toCell != (DBObjRecord *) NULL) && ((toCell->Flags() & DBObjectFlagLocked) == DBObjectFlagLocked);
                 toCell = ToCell(fromCell)) {
                fromCell = toCell;
                fromCell->Flags(DBObjectFlagLocked, DBClear);
            }
        }
        switch (CellDirection (cellRec)) {
            case DBNetDirNE:
                fromCell = FromCell(cellRec, DBNetDirE, false);
                if ((fromCell != (DBObjRecord *) NULL) && (CellDirection(fromCell) == DBNetDirNW)) {
                    if (cellRec->RowID() < fromCell->RowID())
                        CellDirection(fromCell, DBNetDirN);
                    else CellDirection(cellRec, DBNetDirN);
                    changed = true;
                }
                break;
            case DBNetDirSE:
                fromCell = FromCell(cellRec, DBNetDirS, false);
                if ((fromCell != (DBObjRecord *) NULL) && (CellDirection(fromCell) == DBNetDirNE)) {
                    if (cellRec->RowID() < fromCell->RowID())
                        CellDirection(fromCell, DBNetDirE);
                    else CellDirection(cellRec, DBNetDirE);
                    changed = true;
                }
                break;
            case DBNetDirSW:
                fromCell = FromCell(cellRec, DBNetDirW, false);
                if ((fromCell != (DBObjRecord *) NULL) && (CellDirection(fromCell) == DBNetDirSE)) {
                    if (cellRec->RowID() < fromCell->RowID())
                        CellDirection(fromCell, DBNetDirS);
                    else CellDirection(cellRec, DBNetDirS);
                    changed = true;
                }
                break;
            case DBNetDirNW:
                fromCell = FromCell(cellRec, DBNetDirN, false);
                if ((fromCell != (DBObjRecord *) NULL) && (CellDirection(fromCell) == DBNetDirSW)) {
                    if (cellRec->RowID() < fromCell->RowID())
                        CellDirection(fromCell, DBNetDirW);
                    else CellDirection(cellRec, DBNetDirW);
                    changed = true;
                }
                break;
            default:
                break;
        }
    }
    if (changed) goto Rebuild;
    for (i = 0; i < CellNum(); ++i) {
        cellRec = CellTable->Item(i);
        if ((basinRec = Basin(cellRec)) == (DBObjRecord *) NULL) {
            CMmsgPrint(CMmsgAppError, "BasinID: %d CellID:%d", BasinFLD->Int(cellRec), cellRec->RowID());
            continue;
        }
        if (basin != basinRec->RowID()) {
            basin = basinRec->RowID();
            DBPause(80 + basin * 20 / BasinNum());
            ColorFLD->Int(basinRec, 7);
        }
        for (dir = 0; dir < 8; dir += 2) {
            if ((fromCell = FromCell(cellRec, 0x01 << dir, false)) == (DBObjRecord *) NULL) continue;
            if (basinRec == Basin(fromCell)) continue;
            if (ColorFLD->Int(basinRec) == ColorFLD->Int(Basin(fromCell))) {
                ColorFLD->Int(basinRec, ColorFLD->Int(basinRec) + 1);
                cellRec = MouthCell(basinRec);
                i = cellRec->RowID() - 1;
            }
        }
    }
    DBPause(100);
    if (DistToMouth()) SetDistToMouth();
    if (DistToOcean()) SetDistToOcean();
    if (Magnitude())   SetMagnitude();
    return (DBSuccess);
}

int DBNetworkIF::Trim() {
    DBInt i, row, col;
    DBPosition pos, min, max;
    DBRegion extent = DataPTR->Extent();
    DBObjRecord *cellRec, *basinRec;
    DBCoordinate coord;

    min.Row = RowNum();
    min.Col = ColNum();
    max.Row = max.Col = 0;

    for (i = 0; i < CellNum(); ++i) {
        DBPause(33 * i / CellNum());
        cellRec = CellTable->Item(i);
        pos = CellPosition(cellRec);
        min.Row = min.Row < pos.Row ? min.Row : pos.Row;
        min.Col = min.Col < pos.Col ? min.Col : pos.Col;
        max.Row = max.Row > pos.Row ? max.Row : pos.Row;
        max.Col = max.Col > pos.Col ? max.Col : pos.Col;
    }

    for (i = 0; i < CellNum(); ++i) {
        DBPause(33 + 33 * i / CellNum());
        cellRec = CellTable->Item(i);
        pos = CellPosition(cellRec);
        pos.Row = pos.Row - min.Row;
        pos.Col = pos.Col - min.Col;
        PositionFLD->Position(cellRec, pos);
    }
    for (i = 0;i < BasinNum(); ++i) {
        basinRec = Basin (i);
        pos = MouthPosFLD->Position (basinRec);
        pos.Row = pos.Row - min.Row;
        pos.Col = pos.Col - min.Col;
        MouthPosFLD->Position (basinRec,pos);
    }
    coord.X = min.Col * CellWidth();
    coord.Y = min.Row * CellHeight();
    extent.LowerLeft = extent.LowerLeft + coord;
    coord.X = (ColNum() - max.Col - 1) * CellWidth();
    coord.Y = (RowNum() - max.Row - 1) * CellHeight();
    extent.UpperRight = extent.UpperRight - coord;
    DataPTR->Extent(extent);

    row = max.Row - min.Row + 1;
    col = max.Col - min.Col + 1;
    RowNumFLD->Int(LayerRecord, row);
    ColNumFLD->Int(LayerRecord, col);

    DataRec->Realloc((size_t) row * (size_t) col * sizeof(int));

    for (row = 0; row < RowNum(); row++)
        for (col = 0; col < ColNum(); col++)
            ((DBInt *) DataRec->Data())[row * ColNum() + col] = DBFault;

    for (i = 0; i < CellNum(); ++i) {
        DBPause(67 + 33 * i / CellNum());
        pos = CellPosition(CellTable->Item(i));
        ((DBInt *) DataRec->Data())[(size_t) pos.Row * (size_t) ColNum() + (size_t) pos.Col] = cellRec->RowID();
    }

    return (DBSuccess);
}

void DBNetworkIF::SetDistToMouth() {
    DBInt cellID;
    DBObjRecord *cellRec, *upCellRec;

    if (DistToMouthFLD == (DBObjTableField *) NULL) {
        DistToMouthFLD = new DBObjTableField(DBrNDistToMouth, DBTableFieldFloat, "%8.0f", sizeof(DBFloat));
        CellTable->AddField(DistToMouthFLD);
    }

    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        DistToMouthFLD->Float(cellRec, CellLength(cellRec));
    }

    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        DBPause(cellID * 100 / CellNum());
        if ((upCellRec = FromCell(cellRec)) != (DBObjRecord *) NULL)
            DistToMouthFLD->Float(upCellRec, DistToMouthFLD->Float(upCellRec) + DistToMouthFLD->Float(cellRec));
    }
}

void DBNetworkIF::SetDistToOcean() {
    DBInt cellID, dir;
    DBObjRecord *cellRec, *upCellRec;

    if (DistToOceanFLD == (DBObjTableField *) NULL) {
        DistToOceanFLD = new DBObjTableField(DBrNDistToOcean, DBTableFieldFloat, "%8.0f", sizeof(DBFloat));
        CellTable->AddField(DistToOceanFLD);
    }

    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        DistToOceanFLD->Float(cellRec, CellLength(cellRec));
    }

    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        DBPause(cellRec->RowID() * 100 / CellNum());
        for (dir = 0; dir < 8; ++dir)
            if ((upCellRec = FromCell(cellRec, 0x01 << dir)) != (DBObjRecord *) NULL)
                DistToOceanFLD->Float(upCellRec, DistToOceanFLD->Float(upCellRec) + DistToOceanFLD->Float(cellRec));
    }
}

void DBNetworkIF::SetMagnitude() {
    DBInt cellID;
    DBObjRecord *cellRec, *toCellRec;
    if (MagnitudeFLD == (DBObjTableField *) NULL) {
        MagnitudeFLD = new DBObjTableField(DBrNMagnitude, DBTableFieldInt, "%4d", sizeof(DBShort));
        CellTable->AddField(MagnitudeFLD);
    }
    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        MagnitudeFLD->Int(cellRec, (FromCellDirs(cellRec) == 0x0) ? 1 : 0);
    }
    for (cellID = 0; cellID < CellNum(); ++cellID) {
        cellRec = Cell(cellID);
        DBPause((CellNum() - cellRec->RowID()) * 100 / CellNum());
        if ((toCellRec = ToCell(cellRec)) != (DBObjRecord *) NULL)
            MagnitudeFLD->Int(toCellRec, MagnitudeFLD->Int(toCellRec) + MagnitudeFLD->Int(cellRec));
    }
}
