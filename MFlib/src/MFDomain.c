/******************************************************************************

GHAAS Water Balance Model Library V1.0
Global Hydrologic Archive and Analysis System
Copyright 1994-2021, UNH - ASRC/CUNY

MFDomain.c

bfekete@gc.cuny.edu

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cm.h>
#include <MF.h>

void MFDomainFree (MFDomain_p domain) {
	int objID;

	if (domain->Objects != (MFObject_p) NULL) {
		for (objID = 0;objID < domain->ObjNum;++objID) {
			if (domain->Objects [objID].DLinks != (size_t *) NULL) free (domain->Objects [objID].DLinks);
			if (domain->Objects [objID].ULinks != (size_t *) NULL) free (domain->Objects [objID].ULinks);
			if (domain->Objects [objID].DWeights != (float *) NULL) free (domain->Objects [objID].DWeights);
			if (domain->Objects [objID].UWeights != (float *) NULL) free (domain->Objects [objID].UWeights);
		}
		free (domain->Objects);
	}
	free (domain);	
}

MFDomain_p MFDomainRead (FILE *inFile) {
	int objID, i;
	MFDomain_p domain;

	if ((domain = (MFDomain_p) calloc (1,sizeof (MFDomain_t))) == (MFDomain_p) NULL) return ((MFDomain_p) NULL);
	domain->Objects = (MFObject_p) NULL;

	if (fread (domain,sizeof (MFDomain_t) - sizeof (MFObject_p),1,inFile) != 1) {
		CMmsgPrint (CMmsgSysError,"File Reading Error in: %s:%d",__FILE__,__LINE__);
		MFDomainFree (domain);
		return ((MFDomain_p) NULL);
	}
	if (domain->Swap != 1) {
		MFSwapHalfWord (&(domain->Type));		
		MFSwapWord (&(domain->ObjNum));
	}
	if ((domain->Objects = (MFObject_p) calloc (domain->ObjNum,sizeof (MFObject_t))) == (MFObject_p) NULL) {
		CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
		MFDomainFree (domain);
		return ((MFDomain_p) NULL);
	}
	for (objID = 0;objID < domain->ObjNum;++objID) {
		domain->Objects [objID].DLinks = (size_t *) NULL;
		domain->Objects [objID].ULinks = (size_t *) NULL;
		domain->Objects [objID].DWeights = (float *) NULL;
		domain->Objects [objID].UWeights = (float *) NULL;
	}
	for (objID = 0;objID < domain->ObjNum;++objID) {
		if (fread (domain->Objects + objID,sizeof (MFObject_t) - 2 * sizeof (MFObject_p),1,inFile) != 1) {
			CMmsgPrint (CMmsgSysError,"File Reading Error in: %s:%d",__FILE__,__LINE__);
			MFDomainFree (domain);
			return ((MFDomain_p) NULL);
		}
		if (domain->Swap != 1) {
			MFSwapWord (&(domain->Objects [objID].ID));		
			MFSwapHalfWord (&(domain->Objects [objID].DLinkNum));		
			MFSwapHalfWord (&(domain->Objects [objID].ULinkNum));		
			MFSwapWord (&(domain->Objects [objID].XCoord));		
			MFSwapWord (&(domain->Objects [objID].YCoord));		
			MFSwapWord (&(domain->Objects [objID].Lon));		
			MFSwapWord (&(domain->Objects [objID].Lat));		
			MFSwapWord (&(domain->Objects [objID].Area));		
			MFSwapWord (&(domain->Objects [objID].Length));		
		}
		if (domain->Objects [objID].DLinkNum > 0) {
			domain->Objects [objID].DLinks = (size_t *) calloc (domain->Objects [objID].DLinkNum,sizeof (size_t *));
			if (domain->Objects [objID].DLinks == (size_t *) NULL) {
				CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
			domain->Objects [objID].DWeights = (float *) calloc (domain->Objects [objID].DLinkNum,sizeof (float));
			if (domain->Objects [objID].DWeights == (float*) NULL) {
				CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
			if (fread (domain->Objects [objID].DLinks,sizeof (size_t),domain->Objects [objID].DLinkNum,inFile) == domain->Objects [objID].DLinkNum) {
				if (domain->Swap != 1)
					for (i = 0;i < domain->Objects [objID].DLinkNum; ++i)
						MFSwapWord (domain->Objects [objID].DLinks + i);
                for (i = 0; i < domain->Objects [objID].DLinkNum; ++i) {
                    // assume network originates with single downlink.
                    if (i>1) CMmsgPrint(CMmsgSysError, "Bifurcation code assumption of single inital downlink WRONG at %s:%d",__FILE__,__LINE__);
                    domain->Objects[objID].DWeights[i] = 1.0;
                }
			}
			else {
				CMmsgPrint (CMmsgSysError,"File Reading Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
		}
		if (domain->Objects [objID].ULinkNum > 0) {
			domain->Objects [objID].ULinks = (size_t *) calloc (domain->Objects [objID].ULinkNum,sizeof (size_t));
			if (domain->Objects [objID].ULinks == (size_t *) NULL) {
				CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
			domain->Objects [objID].UWeights = (float *) calloc (domain->Objects [objID].ULinkNum,sizeof (float));
			if (domain->Objects [objID].UWeights == (float*) NULL) {
				CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
			if (fread (domain->Objects [objID].ULinks,sizeof (size_t),domain->Objects [objID].ULinkNum,inFile) == domain->Objects [objID].ULinkNum) {
				if (domain->Swap != 1)
					for (i = 0;i < domain->Objects [objID].ULinkNum; ++i)
						MFSwapWord (domain->Objects [objID].ULinks + i);
                for (i = 0; i < domain->Objects [objID].ULinkNum; ++i) {
                    // Weights refer to fraction of UPSTREAM CELL outflow. So all the UWeights could be more or less than 1, but all the DWeights for a cell sum to 1.
                    // again, assuming single downlinks, so initial weight of each downlink is 1
                    domain->Objects[objID].UWeights[i] = 1.0;
                }
			}
			else {
				CMmsgPrint (CMmsgSysError,"File Reading Error in: %s:%d",__FILE__,__LINE__);
				MFDomainFree (domain);
				return ((MFDomain_p) NULL);
			}
		}
	}
	return (domain);
}

int MFDomainSetBifurcations(MFDomain_p domain, const char *path) {
    char line[1024];
    int fromID, toID, dlink, dlink0, ulink, objID;
    float weight, sum;

    FILE *inFile;

    if ((inFile = fopen (path,"r")) == (FILE *) NULL) {
		CMmsgPrint (CMmsgSysError,"Bifurfaction file [%s] Opening error!",CMfileName (path));
		return (CMfailed);
    }

    while (fscanf(inFile, "%d,%d,%f", &fromID, &toID, &weight) == 3) {
        CMmsgPrint(CMmsgDebug, "Read BIFURCATION: fromID: %d, toID: %d, weight: %f", fromID, toID, weight);
        fromID--; // RGIS GUI uses 1-based indexing for cell numbers, we need 0-based
        toID--;

        if (weight == 0.0) { // remove dlink, dweight from fromID. remove ulink, uweight from toID
            domain->Objects[fromID].DLinkNum--;
            if (domain->Objects[fromID].DLinkNum == 0) {
                free(domain->Objects[fromID].DLinks);
                domain->Objects[fromID].DLinks = (size_t *) NULL;
                free(domain->Objects[fromID].DWeights);
                domain->Objects[fromID].DWeights = (float *) NULL;
            } else {
                for (dlink=0; dlink < domain->Objects[fromID].DLinkNum+1; dlink++)
                    if (domain->Objects[fromID].DLinks[dlink] == (size_t) toID)
                        break;
                if (domain->Objects[fromID].DLinks[dlink] != (size_t) toID) {
                    CMmsgPrint (CMmsgSysError,"Bifur input data bad. fromID: %d, toID: %d, not initially downlinked, in: %s:%d", toID+1, fromID+1, __FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
                // remove dlink by moving rest of array up, realloc to smaller size
                for (int _dlink = dlink; _dlink < domain->Objects[fromID].DLinkNum+1; _dlink++) {
                    domain->Objects[fromID].DLinks[_dlink] = domain->Objects[fromID].DLinks[_dlink+1];
                }
                domain->Objects[fromID].DLinks = (size_t *) realloc (domain->Objects[fromID].DLinks, domain->Objects [fromID].DLinkNum * sizeof (size_t));
                if (domain->Objects[fromID].DLinks == (size_t *) NULL) {
                    CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
                // remove dweight by moving rest of array up, realloc to smaller size
                for (int _dlink = dlink; _dlink < domain->Objects[fromID].DLinkNum+1; _dlink++) {
                    domain->Objects[fromID].DWeights[_dlink] = domain->Objects[fromID].DWeights[_dlink+1];
                }
                domain->Objects[fromID].DWeights = (float *) realloc (domain->Objects[fromID].DWeights, domain->Objects [fromID].DLinkNum * sizeof (float));
                if (domain->Objects[fromID].DWeights == (float *) NULL) {
                    CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
            }

            domain->Objects[toID].ULinkNum--;
            if (domain->Objects[toID].ULinkNum == 0) {
                free(domain->Objects[toID].ULinks);
                domain->Objects[toID].ULinks = (size_t *) NULL;
                free(domain->Objects[toID].UWeights);
                domain->Objects[toID].UWeights = (float *) NULL;
            } else {
                for (ulink=0; ulink < domain->Objects[toID].ULinkNum+1; ulink++) {
                    if (domain->Objects[toID].ULinks[ulink] == (size_t) fromID) {
                        break;
                    }
                }
                if (domain->Objects[toID].ULinks[ulink] != (size_t) fromID) {
                    CMmsgPrint (CMmsgSysError,"Bifur input data bad. toID: %d, fromID: %d, not initially uplinked, in: %s:%d", toID+1, fromID+1, __FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
                // remove ulink by moving rest of array up, realloc to smaller size
                for (int _ulink = ulink; _ulink < domain->Objects[toID].ULinkNum+1; _ulink++) {
                    domain->Objects[toID].ULinks[_ulink] = domain->Objects[toID].ULinks[_ulink+1];
                }
                domain->Objects[toID].ULinks = (size_t *) realloc (domain->Objects[toID].ULinks, domain->Objects [toID].ULinkNum * sizeof (size_t));
                if (domain->Objects[toID].ULinks == (size_t *) NULL) {
                    CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
                // remove uweight by moving rest of array up, realloc to smaller size
                for (int _ulink = ulink; _ulink < domain->Objects[toID].ULinkNum+1; _ulink++) {
                    domain->Objects[toID].UWeights[_ulink] = domain->Objects[toID].UWeights[_ulink+1];
                }
                domain->Objects[toID].UWeights = (float *) realloc (domain->Objects[toID].UWeights, domain->Objects [toID].ULinkNum * sizeof (float));
                if (domain->Objects[toID].UWeights == (float *) NULL) {
                    CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                    fclose(inFile);
                    return (CMfailed);
                }
            }
        } else { // weight > 0, add link
            domain->Objects[fromID].DLinkNum++;
            if (domain->Objects[fromID].DLinks == (size_t *) NULL) {
                domain->Objects [fromID].DLinks = (size_t *) calloc (domain->Objects [fromID].DLinkNum, sizeof (size_t *));
            } else {
                domain->Objects[fromID].DLinks = (size_t *) realloc (domain->Objects[fromID].DLinks, domain->Objects [fromID].DLinkNum * sizeof (size_t));
            }
            if (domain->Objects[fromID].DLinks == (size_t *) NULL) {
                CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                fclose(inFile);
                return (CMfailed);
            }
            domain->Objects[fromID].DLinks[domain->Objects[fromID].DLinkNum-1] = (size_t) toID;

            if (domain->Objects[fromID].DWeights == (float *) NULL) {
                domain->Objects [fromID].DWeights = (float *) calloc (domain->Objects [fromID].DLinkNum, sizeof (float));
            } else {
                domain->Objects[fromID].DWeights = (float *) realloc (domain->Objects[fromID].DWeights, domain->Objects [fromID].DLinkNum * sizeof (float));
            }
            if (domain->Objects[fromID].DWeights == (float *) NULL) {
                CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                fclose(inFile);
                return (CMfailed);
            }
            domain->Objects[fromID].DWeights[domain->Objects[fromID].DLinkNum-1] = weight;

            domain->Objects[toID].ULinkNum++;
            if (domain->Objects[toID].ULinks == (size_t *) NULL) {
                domain->Objects [toID].ULinks = (size_t *) calloc (domain->Objects [toID].ULinkNum, sizeof (size_t));
            } else {
                domain->Objects[toID].ULinks = (size_t *) realloc (domain->Objects[toID].ULinks, domain->Objects [toID].ULinkNum * sizeof (size_t));
            }
            if (domain->Objects[toID].ULinks == (size_t *) NULL) {
                CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                fclose(inFile);
                return (CMfailed);
            }
            domain->Objects[toID].ULinks[domain->Objects[toID].ULinkNum-1] = (size_t) fromID;

            if (domain->Objects[toID].UWeights == (float *) NULL) {
                domain->Objects [toID].UWeights = (float *) calloc (domain->Objects [toID].ULinkNum, sizeof (float));
            } else {
                domain->Objects[toID].UWeights = (float *) realloc (domain->Objects[toID].UWeights, domain->Objects [toID].ULinkNum * sizeof (float));
            }
            if (domain->Objects[toID].UWeights == (float *) NULL) {
                CMmsgPrint (CMmsgSysError,"Memory Allocation Error in: %s:%d",__FILE__,__LINE__);
                fclose(inFile);
                return (CMfailed);
            }
            domain->Objects[toID].UWeights[domain->Objects[toID].ULinkNum-1] = weight;
        }
    }
    fclose(inFile);

    // check resulting domain, confirm all downlinks sum to 1 for each cell. if not then
    // bifurcation input or code assumptions are wrong
    // Check cell 0 (though other basin outlets will have 0 downlinks)
    if (domain->Objects[0].DLinkNum > 0) {
        CMmsgPrint (CMmsgSysError,"There should be no downlinks for cell 0, in: %s:%d",objID,__FILE__,__LINE__);
        return (CMfailed);
    }
    // Check weights
	for (objID = 0; objID < domain->ObjNum; objID++) {
        if (domain->Objects[objID].DLinkNum == 0) {
            continue;
        }
        sum = 0.0;
        for (dlink=0; dlink < domain->Objects[objID].DLinkNum; dlink++) {
            sum += domain->Objects[objID].DWeights[dlink];
        }
        if (sum != 1.0) {
            CMmsgPrint (CMmsgSysError,"Downlink weights sum to %f (should be 1) for cell %d in: %s:%d",sum, objID,__FILE__,__LINE__);
            return (CMfailed);
        }
    }
    return (CMsucceeded);
}

int MFDomainWrite (MFDomain_p domain,FILE *outFile) {
	int objID;

	domain->Swap = 1;
	if (fwrite (domain,sizeof (MFDomain_t) - sizeof (MFObject_p),1,outFile) != 1) {
		CMmsgPrint (CMmsgSysError,"File Writing Error in: %s:%d",__FILE__,__LINE__);
		return (CMfailed);
	}

	for (objID = 0;objID < domain->ObjNum;++objID) {
		if (fwrite (domain->Objects + objID,sizeof (MFObject_t) - 2 * sizeof (MFObject_p),1,outFile) != 1) {
			CMmsgPrint (CMmsgSysError,"File Writng Error in: %s:%d",__FILE__,__LINE__);
			return (CMfailed);
		}
		if (domain->Objects [objID].DLinkNum > 0) {
			if (fwrite (domain->Objects [objID].DLinks,sizeof (size_t),domain->Objects [objID].DLinkNum,outFile) != domain->Objects [objID].DLinkNum) {
				CMmsgPrint (CMmsgSysError,"File Writing Error in: %s:%d",__FILE__,__LINE__);
				return (CMfailed);
			}
		}
		if (domain->Objects [objID].ULinkNum > 0) {
			if (fwrite (domain->Objects [objID].ULinks,sizeof (size_t),domain->Objects [objID].ULinkNum,outFile) != domain->Objects [objID].ULinkNum) {
				CMmsgPrint (CMmsgSysError,"File Writing Error in: %s:%d",__FILE__,__LINE__);
				return (CMfailed);
			}
		}
	}
	return (CMsucceeded);
}
