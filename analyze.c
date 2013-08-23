#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>
#include <assert.h>

OscillatorDesc oscillation(LifeList *cells, LifeList *working, int testUpTo) {

  OscillatorDesc osc;
  int i;

  Transformation normT;
  Transformation oscT;

  normT= normalize(cells);

  resizeIfNeeded(working, cells->ncells);
  working->ncells= copyList(cells->cellList, cells->ncells, 
			    working->cellList, 0);

  setValues(working->cellList, working->ncells, 1);

  for (i=1; i<=testUpTo; i++) {

    generate(working);
    if (cells->ncells==working->ncells) {
      oscT= normalize(working); 

      if ( !compare(working->cellList, cells->cellList, cells->ncells)) 
	break;
      transformBack(working->cellList, oscT, working->ncells);
    }

  }

  osc.testedUpTo = testUpTo;

  if (i > testUpTo) osc.period= -1;
  else osc.period= i; 

  osc.T= oscT;

  transformBack(cells->cellList, normT, cells->ncells);


  return osc;
}

OscillatorDesc simpleOscillation(LifeList *cells, LifeList *working, 
                                 int testUpTo) {
 
   int i, initTransl, transl;
   OscillatorDesc osc;
 
   /* Looks for first "simple" oscillation of a pattern up to testUpTo.
      This includes a translation, but no other coordinate transformation.
      Returns the period and translation (returns -1 if no oscillation found) */
 
   initTransl = firstZero(cells->cellList,cells->ncells);
   copyLifeList(cells, working);
 
   for (i=1; i<=testUpTo; i++) {
      generate(working);
 
      if (cells->ncells == working->ncells) {
         transl = firstZero(working->cellList, working->ncells);

         if (!compare(working->cellList, cells->cellList, cells->ncells)) 
                                                                    break;
         copyList(working->cellList, working->ncells, 
                  working->cellList, transl);
       }
       
   }

   copyList(cells->cellList, cells->ncells, cells->cellList, initTransl);

   osc.testedUpTo = testUpTo;

   if (i> testUpTo) osc.period = -1; 
   else osc.period = i; 

   osc.T.flipxF = osc.T.flipyF = osc.T.transposeF = 0;

   osc.T.translateBy = transl;

   return osc;
}

int simpleSS(LifeList *cells, LifeList *working) {

   int i, transl;

/* checks if pattern in cells is a simple (period 4) spaceship */

   (void) firstZero(cells->cellList,cells->ncells); 

   copyLifeList(cells, working);

   for (i=0; i<4; i++) generate(working);
  
   transl = firstZero(working->cellList, working->ncells); 

   if (cells->ncells == working->ncells &&
      !compare(working->cellList, cells->cellList, cells->ncells)) {
        return transl;
      }
   else return packtrans(-255, -255);

}


Cell *scratch1, *scratch2, *convolution, *oldAlignments;
int maxscratch=0;

void makeWorkSpace(int n) {

  if (n >= maxscratch) {

    if (maxscratch==0) {
       scratch1= NULL;
       scratch2= NULL;
       convolution= NULL;
       oldAlignments= NULL;
    }

    maxscratch= 2*n+1;

/*    logcomment("allocating scratch space"); */
    scratch1= (Cell *)realloc(scratch1, sizeof(Cell)*maxscratch);
    scratch2= (Cell *)realloc(scratch2, sizeof(Cell)*maxscratch);
    convolution= (Cell *)realloc(convolution, sizeof(Cell)*maxscratch);
    oldAlignments= (Cell *)realloc(oldAlignments, sizeof(Cell)*maxscratch);

  } 

}

initMatchList(PartialOscillatorDesc *osc, int nmatches) {

int i;

   for (i=0; i<nmatches+1; i++) osc[i].matchDistance=INT_MAX;

}

void bestMatches(LifeList *cells1, LifeList *cells2, int period, 
                     int nmatches, PartialOscillatorDesc *osc) {
 
  int nConvolution, nEmpty;
  int i;
  Transformation T, bestT;

  setValues(cells1->cellList, cells1->ncells, 1);
  setValues(cells2->cellList, cells2->ncells, 1);

  T.translateBy= 0;
  for (T.flipxF=0; T.flipxF<=1; T.flipxF++) {
    for (T.flipyF=0; T.flipyF<=1; T.flipyF++) {
      for (T.transposeF=0; T.transposeF<=1; T.transposeF++) {

	transform(cells2->cellList, T, cells2->ncells);
	nConvolution=convolve(cells1->cellList, cells1->ncells, 
			      cells2->cellList, cells2->ncells, 
			      &convolution, &scratch1, &scratch2,
                              makeWorkSpace);

   
	for (i=0; i<nConvolution; i++) { 
          
           
	  if (cells1->ncells - convolution[i].value 
                           < osc[nmatches-1].matchDistance) {
            int j,matchDistance; 


            matchDistance = cells1->ncells - convolution[i].value;

            nEmpty =emptyNeighbors(cells1);
            copyList(cells1->neighborhoods, nEmpty,
                     cells1->neighborhoods, convolution[i].position);

            matchDistance+=countMatch(cells1->neighborhoods, nEmpty,
                                      cells2->cellList, cells2->ncells);

            j= nmatches-1;

            while (j>=0 && matchDistance < osc[j].matchDistance) {
                osc[j+1]=osc[j];
                j--; 
            }

            osc[j+1].period=period;
            osc[j+1].T=T;
            osc[j+1].T.translateBy= convolution[i].position;
            osc[j+1].matchDistance=matchDistance;

	  }
	}
	transformBack(cells2->cellList, T, cells2->ncells);
      }
    }
  }
}


void partialOscillation(LifeList *cells, LifeList *working, 
                        int testUpTo, int testPhases,
                        int nmatches, PartialOscillatorDesc *posc) {

  int i,j;
  History hist;
  Transformation normT;
  Transformation oscT;

  normT= normalize(cells);
  hist= makeHistory(cells, testUpTo+testPhases); 

  initMatchList(posc, nmatches);
  for (i=1; i<=testUpTo; i++) {
    int matchSize;
    int tx, ty;

    for (j=0; j<testPhases; j++) {
      getGeneration(cells, hist, j);
      getGeneration(working, hist, i+j);
      bestMatches(cells, working, i, nmatches, posc);
    }

  }

  getGeneration(cells, hist, 0);
  freeHistory(hist);
  transformBack(cells->cellList, normT, cells->ncells);

}

/* Note: only used by firstTouch.  Returns result in
   global convolution */
int findTouching(LifeList *cells1, LifeList *cells2) {

  int n1, n2, n;

    n1=sumNeighbors(cells1); 
    setValues(cells1->neighborhoods, n1, 1);

    n2=sumNeighbors(cells2); 
    setValues(cells2->neighborhoods, n2, 1);

    n=convolve(cells1->neighborhoods, n1,
               cells2->neighborhoods, n2,
               &convolution, &scratch1, &scratch2,
               makeWorkSpace);    
 
    return n;
}


AlignmentList
   firstTouch(LifeList *cells1, LifeList *cells2, int firstGen, int ngens) {
int nOld=0, nNew;
int i, lastGen;

AlignmentList list;

    lastGen=firstGen+ngens;

    assert(firstGen<=lastGen);

    list.alignments= (Cell **)calloc(sizeof(Cell *), lastGen-firstGen);
    list.firstGen= firstGen;
    list.ngens= ngens;

    list.nalignments= (int *)calloc(sizeof(int), lastGen-firstGen);

    for (i=0; i<firstGen; i++) {
        nNew= findTouching(cells1, cells2);

        makeWorkSpace(nNew+nOld); 
        nOld=combineLists(convolution, nNew, oldAlignments, nOld, scratch1);
        copyList(scratch1, nOld, oldAlignments, 0);
        setValues(oldAlignments, nOld, 1);

        generate(cells1);
        generate(cells2);
    }

    for (; i<lastGen; i++) {
        nNew= findTouching(cells1, cells2);

        nNew= subtractLists(oldAlignments, nOld, convolution, nNew);
        list.alignments[i-firstGen]=
              (Cell *)calloc(sizeof(Cell), nNew);
        list.nalignments[i-firstGen]=
               copyList(convolution, nNew, list.alignments[i-firstGen], 0); 

        makeWorkSpace(nNew+nOld); 
        nOld=combineLists(convolution, nNew, oldAlignments, nOld, scratch1);
        copyList(scratch1, nOld, oldAlignments, 0);
        setValues(oldAlignments, nOld, 1);

        generate(cells1);
        generate(cells2);
     }

     return list;
}

void freeAlignmentList(AlignmentList list) {
    int i;

    for (i=0; i<list.ngens; i++) {
        free(list.alignments[i]);
    }
    free(list.alignments);
    free(list.nalignments);
}

void dumpAlignments(AlignmentList list) {

    int i;

    for (i=list.firstGen; i<list.firstGen+list.ngens; i++) {
       printf("generation: %3d # alignments: %3d\n", i, 
                         list.nalignments[i-list.firstGen]);
       dumpTransCells(list.alignments[i-list.firstGen], 
                      list.nalignments[i-list.firstGen]);
    }
}


int emptyNeighbors(LifeList *cells) {
int nnbhd;

   resizeIfNeeded(cells, cells->ncells*9);

   nnbhd= sumAllNeighbors(cells->cellList, cells->ncells, 
                                           cells->neighborhoods);
   nnbhd= subtractLists(cells->cellList, cells->ncells, 
                                cells->neighborhoods, nnbhd);

   return nnbhd;
}


int minNeighbors(LifeList *cells) {

  int n1, n2, n3;

  makeWorkSpace(2*cells->ncells);
  n1=copyList(cells->cellList, cells->ncells, scratch1, -INCY);
  n2= combineListsMin(cells->cellList, cells->ncells,
                          scratch1, n1, scratch2);
  
  n1=copyList(cells->cellList, cells->ncells, scratch1,  INCY);
  resizeIfNeeded(cells, n1+n2);
  n3= combineListsMin(scratch1, n1,
                      scratch2, n2, cells->neighborhoods);

  makeWorkSpace(2*n3);
  n1=copyList(cells->neighborhoods, n3, scratch1, -INCX);
  n2= combineListsMin(scratch1, n1,
                      cells->neighborhoods, n3, scratch2);

  n1=copyList(cells->neighborhoods, n3, scratch1,  INCX);
  resizeIfNeeded(cells, n1+n2);
  n3= combineListsMin(scratch1, n1,
                      scratch2, n2, cells->neighborhoods);

  return n3;
}

Transformation normalize(LifeList *cells) {

  Transformation T;
  Transformation normalizeT;

  makeWorkSpace(cells->ncells);

  scratch1[0].position=INT_MAX;

  for (T.flipxF=1; T.flipxF>=0; T.flipxF--) {
    for (T.flipyF=1; T.flipyF>=0; T.flipyF--) {
      for (T.transposeF=1; T.transposeF>=0; T.transposeF--) {

        copyList(cells->cellList, cells->ncells, scratch2, 0);

        if (T.flipxF) flipx(scratch2, cells->ncells);
        if (T.flipyF) flipy(scratch2, cells->ncells);
        if (T.transposeF) transpose(scratch2, cells->ncells);

        T.translateBy=firstZero(scratch2, cells->ncells);

        if (compare(scratch2, scratch1,
                    cells->ncells) <= 0) {
          copyList(scratch2, cells->ncells, scratch1, 0);
          normalizeT=T;
        }
      }
    }
  }

  copyList(scratch1, cells->ncells, cells->cellList, 0);

  return normalizeT;

}

int trail(LifeList *cells, int ngen, Cell *trail, int ntrail, int gen) {

   int i;

   for (i=0; i<ngen; i++) { 

       setValues(cells->cellList, cells->ncells, i+gen);

       makeWorkSpace(cells->ncells+ntrail);
       ntrail=combineListsMin(cells->cellList, cells->ncells, 
                             trail, ntrail, scratch1); 
 
       copyList(scratch1, ntrail, trail, 0);

       generate(cells);
   }

   setValues(cells->cellList, cells->ncells, 1);
   return ntrail;
}

void spread(LifeList *cells, int distance) {
int k,n;

       for (k=0; k<distance; k++) {
          n= sumNeighbors(cells);
          cells->ncells=
              copyList(cells->neighborhoods, n, cells->cellList, 0);
          setValues(cells->cellList, n, 1);
       }
}


History makeHistory(LifeList *cells, int ngenerations) {

    int i;
    History hist;

    hist.cellList= (Cell **)calloc(sizeof(Cell *), ngenerations);
    hist.ncells= (int *)calloc(sizeof(int), ngenerations);
    hist.ngenerations= ngenerations;
    hist.ntrail=0;

    setValues(cells->cellList, cells->ncells, 1);

    for (i=0; i<ngenerations; i++) {
       hist.cellList[i]=(Cell *)calloc(sizeof(Cell), cells->ncells+1);

       hist.ncells[i]=
          copyList(cells->cellList, cells->ncells, hist.cellList[i], 0);

       makeWorkSpace(hist.ntrail+hist.ncells[i]);

       hist.ntrail=trail(cells, 1, scratch2, hist.ntrail, i);
    }

    hist.trail=(Cell *)calloc(sizeof(Cell *), hist.ntrail+1);
    copyList(scratch2, hist.ntrail, hist.trail, 0);

    return hist;
}




void freeHistory(History hist) {
    int i;

    for (i=0; i<hist.ngenerations; i++) {
        free(hist.cellList[i]);
    }
    free(hist.cellList);
    free(hist.ncells);
    free(hist.trail);
}

void dumpHistory(History hist) {

    int i;

    for (i=0; i<hist.ngenerations; i++) {
       printf("generation: %3d cellcount: %3d\n", i, hist.ncells[i]);
       dumpCells(hist.cellList[i], hist.ncells[i]);
    }
}

void getGeneration(LifeList *cells, History hist, int gen) {

    assert(gen < hist.ngenerations);

    resizeIfNeeded(cells, hist.ncells[gen]);

    cells->ncells=
        copyList(hist.cellList[gen], hist.ncells[gen], cells->cellList, 0);

}

static LifeList spread1, spread2, cells, perturbcells, tmp, thisperturb;

int mostSurvives(LifeList *pattern, LifeList *perturb, int transl, int steps) {

int i;

   copyLifeList(pattern, &tmp);

   copyLifeList(perturb, &spread1);
   spread(&spread1, 1);

   for (i=0; i<steps; i++) { 
        generate(&tmp);
   }

   if ( matchLifeList(&tmp, perturb, transl) >= perturb->ncells-4 ) 
                          return 1;

   return 0;

}

int survives(LifeList *pattern, LifeList *perturb, int transl, int steps)
{

int i;

   copyLifeList(pattern, &tmp);

   copyLifeList(perturb, &spread1);
   spread1.ncells = removeGreaterThan(spread1.cellList, spread1.ncells, 1);

   for (i=0; i<steps; i++) { 
        generate(&tmp);
        if ( matchLifeList(&tmp, &spread1, transl) < spread1.ncells )
        //  || matchLifeList (&tmp, perturb, transl) < perturb->ncells-6  ) 
                        return 0;
   }

   return 1;

}

int restored(LifeList *pattern, LifeList *perturb, int transl, int steps) {
int i,damaged, lastsfor;

   copyLifeList(perturb, &spread1);
   spread(&spread1, 1);

   copyLifeList(pattern, &tmp);

   damaged = 0;
   for (i=0; i<steps; i++) { 
        generate(&tmp);
        if ( matchLifeList(&tmp, perturb, transl) == perturb->ncells && 
             matchLifeList(&tmp, &spread1, transl) == perturb->ncells ) {
           if (damaged) lastsfor++;
           if (lastsfor>=10) return 1;
        } else {
           damaged = 1;
           lastsfor = 0;
        }
   }

   return 0;
}



void setupPerturbors(LifeList *perturbs, Cell *placed, int nplaced,
                    LifeList *justPerturbs, LifeList *reaction) {
  int i;

  justPerturbs->ncells=0;
  if (nplaced==0) {
    resizeIfNeeded(justPerturbs, 1);
  } else {

    for (i=0; i<nplaced; i++) {
      mergeLifeLists(justPerturbs, perturbs+placed[i].value,
		     placed[i].position);
    }
    mergeLifeLists(reaction, justPerturbs, 0);

  }
}


int placeNewPerturbor(LifeList *seed, LifeList *perturbs,
                       Cell *placed, int nplaced, int newperturb,
                       int initialGen, int finalGen, Cell *aligns,
                       int maxdamage) {
  int i, j;
  int  nconv, nold, naligns, nelim;
  int damage;

  copyLifeList(perturbs+newperturb, &thisperturb);

  copyLifeList(&thisperturb, &spread1);
  spread(&spread1, 1);

  copyLifeList(&spread1, &spread2);
  spread(&spread2, 1);

  removeLifeList(&spread2, &spread1, 0);

  copyLifeList(seed, &cells);

  setupPerturbors(perturbs, placed, nplaced, &perturbcells, &cells);

  naligns=0;
  damage=0;
  for (i=0; i<finalGen; i++) {

    nconv=convolve(spread2.cellList, spread2.ncells,
		   cells.cellList, cells.ncells,
		   &convolution, &scratch1, &scratch2,
                   makeWorkSpace);

    makeWorkSpace(spread1.ncells*cells.ncells);
    nold=convolve(spread1.cellList, spread1.ncells,
		  cells.cellList, cells.ncells,
		  &oldAlignments, &scratch1, &scratch2,
                  makeWorkSpace);

    nconv=subtractLists(oldAlignments, nold, convolution, nconv);

    nconv=subtractLists(aligns, naligns, convolution, nconv);

    setValues(convolution, nconv, i);

    nelim=0;
    for (j=0; j<nconv; j++) {
      copyLifeList(&cells, &tmp);
      intersectLifeLists(&tmp, &spread2, convolution[j].position);

      if (interact(&tmp, &thisperturb, convolution[j].position)) {
	convolution[nelim++]=convolution[j];
      }
    }

    naligns=combineLists(convolution, nelim, aligns, naligns, scratch1);
    copyList(scratch1, naligns, aligns, 0);

    generate(&cells);

    if (countMatch(cells.cellList, cells.ncells,
                   perturbcells.cellList, perturbcells.ncells) 
                         < perturbcells.ncells) {
               damage++;
               if (damage>maxdamage) break;
    } else {
      damage=0;
    }
 
  }

  naligns=removeLessThan(aligns, naligns, initialGen);
  return naligns;
}

void cumulativeImage(LifeList *cells, LifeList *cumulative, int ngens) {
int i;


    copyLifeList(cells, cumulative);
    for (i=1; i<ngens; i++) {

       generate(cells);
       mergeLifeLists(cumulative, cells, 0);

    }

} 

int independentUpTo(LifeList *cells, LifeList *cumulative, 
                     LifeList *working, int ngens) {

int nClusters;

   copyLifeList(cells, working);
   cumulativeImage(working, cumulative, ngens);

   nClusters = findComponents(cumulative, working, 2);
  
   getLifeListValues(cells, working); 

   return nClusters;
}

int getClusterDesc(LifeList *cells, LifeList *working1,
                   LifeList *working2, int ngens, 
                   LifeList *clusters, 
                   ClusterDesc *clusterInfo) {
 int i, nClusters;


/* Warning: the calling procedure has to allocate space for
   the cluster information and an initialized array of LifeLists  */

    nClusters = independentUpTo(cells, working1, working2, ngens);

    /* First, partition the list into clusters, maintaining
       the original ordering of cells within clusters */
    for (i=0; i<nClusters; i++) clusters[i].ncells=0;

    for (i=0; i<cells->ncells; i++) {
        LifeList *thiscluster = &(clusters[cells->cellList[i].value]);

        resizeIfNeeded(thiscluster, (thiscluster->ncells)++);
        thiscluster->cellList[thiscluster->ncells-1]=cells->cellList[i];
        thiscluster->cellList[thiscluster->ncells-1].value=1;
    } 

    /* Now collect information about each cluster */
    for (i=0; i<nClusters; i++) {
       int period;

       clusterInfo[i].osc = simpleOscillation(&(clusters[i]), working1, ngens); 
 
       period=clusterInfo[i].osc.period;

       if (period>=1) {
          cumulativeImage(&(clusters[i]), working1, period); 
          clusterInfo[i].bb = makeBoundingBox(working1->cellList, 
                                              working1->ncells); 
       } else {
          clusterInfo[i].bb = makeBoundingBox(clusters[i].cellList, 
                                              clusters[i].ncells); 
       }

    }

    return nClusters;
}

int approaching(ClusterDesc cluster1, ClusterDesc cluster2) {

   int dx1, dy1;
   int dx2, dy2; 

   int rdx, rdy; 
   int xreceding, yreceding;

     unpacktrans(cluster1.osc.T.translateBy, &dx1, &dy1);  
     unpacktrans(cluster2.osc.T.translateBy, &dx2, &dy2);  

     /* Compute relative dx and dy from cluster1's frame of reference,
        corrected for spaceship period */

     rdx=dx2*cluster1.osc.period-dx1*cluster2.osc.period; 
     rdy=dy2*cluster1.osc.period-dy1*cluster2.osc.period; 

     if (rdx==0 && rdy==0) return 0;

     xreceding=0;
     if (rdx < 0) {
        if (cluster1.bb.lx > cluster2.bb.ux+2) xreceding=1;
     } else if (rdx > 0) {
        if (cluster2.bb.lx > cluster1.bb.ux+2) xreceding=1;
     }

     yreceding=0;
     if (rdy < 0) {
        if (cluster1.bb.ly > cluster2.bb.uy+2) yreceding=1;
     } else if (rdy > 0) {
        if (cluster2.bb.ly > cluster1.bb.uy+2) yreceding=1;
     }

     return !(xreceding || yreceding);

}

int possibleInteraction(ClusterDesc *clusterInfo, int nClusters) {
int i, j;

    for (i=0; i<nClusters; i++) {

         /* flag any non-periodic pattern */
         if (clusterInfo[i].osc.period<1) return 1;
       
         /* otherwise consider pairs: at least one pattern must be 
            a spaceship (assume it's the first) */

         if (clusterInfo[i].osc.T.translateBy != 0) {
             /* pair with any higher-indexed spaceship or
                stationary oscillator */
             for (j=0; j<nClusters; j++) {
                 if (j>i || clusterInfo[j].osc.T.translateBy == 0) {
                    if (approaching(clusterInfo[i], clusterInfo[j])) {
                         return 1;
                    } 
                 }
             }
         }
    }

    return 0;
}

