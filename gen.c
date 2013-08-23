/* All code copyright Paul Callahan 1996  (callahan@inf.ethz.ch) */

#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>
#include <assert.h>

int hexNeighborhood=0; 

#ifdef COUNTLOOKUPS
extern int nlookups;
#endif

      int rules[]={
                   0, 0,
                   0, 0,
                   0, 1,
                   1, 1,
                   0, 0,
                   0, 0, 
                   0, 0, 
                   0, 0, 
                   0, 0 };


       int hexTable[2][13] = {
   /*           0   1   2   2'  2'' 3   3'  3'' 4   4'  4'' 5   6   */ 
             {  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
             {  0,  0,  0,  1,  0,  1,  1,  1,  1,  1,  1,  0,  0 } };

int spreadColor[5]={ 0x0000, 0x0001, 0x0010, 0x0100, 0x1000 };

int neighborhoodTable[1<<9];
int changesTable[1<<9];
static int fired[1<<9];
static int initializedTable= 0;

/* maintain history of cell populations for 
   filtering low-period oscillations */
static int pop[POPHISTORYSIZE];
static int repetitions[POPHISTORYSIZE];
int iPop;

/* perform reallocations when list might grow too long */
void resizeIfNeeded(LifeList *cells, int n) {

  if (n >= cells->maxsize) {
    int newsize=2*n+1;

    if (cells->maxsize==0) {
      cells->cellList=NULL;
      cells->neighborhoods=NULL;
    }

/*    logcomment("resizing a LifeList");
*/
    cells->cellList=(Cell *)realloc(cells->cellList, sizeof(Cell)*newsize);
    cells->neighborhoods=(Cell *)realloc(cells->neighborhoods, 
                                   sizeof(Cell)*newsize);

    cells->maxsize=newsize;
/*
    printf("newsize: %d\n", cells->maxsize);
*/
  }
}

void freeLifeList(LifeList *cells) {
  if (cells->maxsize>0) {
      free(cells->cellList);
      free(cells->neighborhoods);

      cells->maxsize=0;
  } 
}

int nextGen(Cell *counts, int ncounts, Cell *cellList) {

   int i, ncells;

   ncells=0;

   for (i=0; i<ncounts; i++) {
     if (rules[counts[i].value]) {
       cellList[ncells].position= counts[i].position;
       cellList[ncells++].value=2;
     }
   }

   return ncells;
 }

int nextGenFromTable(Cell *neighborhoods, int nneighborhoods, Cell *cellList) {

   int i, ncells;

   ncells=0;

   for (i=0; i<nneighborhoods; i++) {
#ifdef COUNTLOOKUPS
     fired[neighborhoods[i].value]++;  
     nlookups++;
#endif
     fired[neighborhoods[i].value]++;  
     if (neighborhoodTable[neighborhoods[i].value]) {
       cellList[ncells].position= neighborhoods[i].position;
       cellList[ncells++].value=1;
     }
   }

   return ncells;
 }

int sumNeighbors(LifeList *cells) {

  int n1, n2, n3;

  resizeIfNeeded(cells, 9*cells->ncells);
  n3=sumAllNeighbors(cells->cellList, cells->ncells, cells->neighborhoods);

  return n3;
}


void makeRowMajor(LifeList *cells) {

   sortRowMajor(cells->cellList, cells->ncells);
   cells->ncells= uniqCells(cells->cellList, cells->ncells);

}

void oscCheck(int n) {

   int i;

  /* low period oscillation check */
  for (i=1; i<=POPHISTORYSIZE; i++) {
     if (pop[(iPop+POPHISTORYSIZE-i)%POPHISTORYSIZE]==n) {
        repetitions[i-1]++;
     } else {
        repetitions[i-1]=0;
     } 
  }
  pop[iPop]= n;
  iPop=(iPop+1)%POPHISTORYSIZE;


}
void initNeighborhoodTable();
void initHexNeighborhoodTable();

int initTransitionTable() {
int i;
FILE *rulefile;

  if (rulefile=fopen("hex.rules", "r")) {

     for (i=0; i<13; i++) {
        fscanf(rulefile, "%d%d", hexTable[0]+i, hexTable[1]+i);
     }

     fclose(rulefile);

     initHexNeighborhoodTable();

  } else {
     if (rulefile=fopen("totalistic.rules", "r")) {

        for (i=0; i<18; i++) {
          fscanf(rulefile, "%d", rules+i);
        }
 
        fclose(rulefile);

     }

     initNeighborhoodTable();

  }

  if (rulefile=fopen("exceptions.rules", "r")) {

     int i, value;
 
      while(1) {
          if (fscanf(rulefile,"%o%d",&i, &value)<2) break;
          neighborhoodTable[i]=value;
      }

      fclose(rulefile);
  }

}

void initNeighborhoodTable() {

  int i, b0, b1, b2, b3, b4, b5, b6, b7, b8;

  hexNeighborhood = 0;

  for (i=0; i<(1<<9); i++) {
     b0=    i  & 1;
     b1=(i>>1) & 1; 
     b2=(i>>2) & 1; 
     b3=(i>>3) & 1; 
     b4=(i>>4) & 1; 
     b5=(i>>5) & 1; 
     b6=(i>>6) & 1; 
     b7=(i>>7) & 1; 
     b8=(i>>8) & 1; 

     neighborhoodTable[i]=rules[b4 +2*(b0+b1+b2+b3+b5+b6+b7+b8)];
     if (neighborhoodTable[i]!=b4) {
       changesTable[i]= (neighborhoodTable[i])?TRANSON:TRANSOFF;
     } else {
       changesTable[i]= NOTRANS;
     }
     fired[i]=0;
  } 

}

void initHexNeighborhoodTable() {

  int i, b0, b1, b2, b3, b4, b5, b6, b7, b8;
  int sum;

  hexNeighborhood = 1;

  for (i=0; i<(1<<9); i++) {
     b0=    i  & 1;
     b1=(i>>1) & 1; 
     b2=(i>>2) & 1; 
     b3=(i>>3) & 1; 
     b4=(i>>4) & 1; 
     b5=(i>>5) & 1; 
     b6=(i>>6) & 1; 
     b7=(i>>7) & 1; 
     b8=(i>>8) & 1; 

     sum = b0+b1+b3+b5+b7+b8;

     if (sum == 0) neighborhoodTable[i] = hexTable[b4][0];
     else if (sum == 1) neighborhoodTable[i] = hexTable[b4][1];
     else if (sum == 5) neighborhoodTable[i] = hexTable[b4][11];
     else if (sum == 6) neighborhoodTable[i] = hexTable[b4][12];
     else if (sum == 4 ) {
        if (
           ( !b0 && !b1 ) || ( !b1 && !b5 ) || (!b5 && !b8) ||
           ( !b8 && !b7 ) || ( !b7 && !b3 ) || (!b3 && !b0)  ) 
         neighborhoodTable[i]= hexTable[b4][8];
        else if (
           ( !b1 && !b8 ) || ( !b8 && !b3 ) || (!b3 && !b1) ||
           ( !b0 && !b5 ) || ( !b5 && !b7 ) || (!b7 && !b0)  ) 
         neighborhoodTable[i]=  hexTable[b4][9];
        else if ( ( !b0 && !b8 ) || ( !b1 && !b7 ) || ( !b3 && !b5 ) ) 
         neighborhoodTable[i]= hexTable[b4][10];

     }
     else if (sum == 3 ) {

         if ( (b1 && b8 && b3) || (b0 && b5 && b7) )  
           neighborhoodTable[i]= hexTable[b4][7];
         else if ( 
                 ( b1 && b5 && b8) ||
                 ( b5 && b8 && b7) ||
                 ( b8 && b7 && b3) ||
                 ( b7 && b3 && b0) ||
                 ( b3 && b0 && b1) ||
                 ( b0 && b1 && b5) ) neighborhoodTable[i]= hexTable[b4][5]; 
         else {
           neighborhoodTable[i]= hexTable[b4][6];
         }
 
     }
     else if (sum == 2 ) {

        if (
           ( b0 && b1 ) || ( b1 && b5 ) || (b5 && b8) ||
           ( b8 && b7 ) || ( b7 && b3 ) || (b3 && b0)  ) 
         neighborhoodTable[i]= hexTable[b4][2];
        else if (
           ( b1 && b8 ) || ( b8 && b3 ) || (b3 && b1) ||
           ( b0 && b5 ) || ( b5 && b7 ) || (b7 && b0)  ) 
         neighborhoodTable[i]= hexTable[b4][3];
        else if ( ( b0 && b8 ) || ( b1 && b7 ) || ( b3 && b5 ) ) 
         neighborhoodTable[i]= hexTable[b4][4];

     }

     if (neighborhoodTable[i]!=b4) {
       changesTable[i]= (neighborhoodTable[i])?TRANSON:TRANSOFF;
     } else {
       changesTable[i]= NOTRANS;
     }
     fired[i]=0;
  } 

}


void generate(LifeList *cells) {

  int i,n;

  if (!initializedTable) {
     initTransitionTable();
     initializedTable= 1;
  }

  resizeIfNeeded(cells, 9*cells->ncells);

  n=findNeighborhoods(cells->cellList, cells->ncells, cells->neighborhoods);

  cells->ncells= nextGenFromTable(cells->neighborhoods, n, cells->cellList);

  oscCheck(cells->ncells);
}

void initPopHistory() {

  int i;

  for (i=0; i<POPHISTORYSIZE; i++) { 
     pop[i]= -1;
     repetitions[i]= 0;     
  }

  iPop= 0;
  
}

void longestOscillation(int *period, int *repetition) {
int i;

   *repetition= -1; 

   for (i=0; i<POPHISTORYSIZE; i++) {   
       if (repetitions[i] > *repetition) {
          *period = i+1;
          *repetition= repetitions[i]; 
       }
   }

}

void dumpfired() {
int i;
int a,b;
int x;

   for (i=0; i< (1<<9); i++) { 
      if (fired[i]) {
         printf("%03o %7d\n", i, fired[i]);
/*
         x=i;
         for (a=0; a<3; a++) { 
           printf("    ");
           for (b=0; b<3; b++) {
              printf("%d", x%2);
              x=x/2; 
           }
           printf("\n");
         }
*/
      }
   }


}

static int *cellCounts=NULL;

int generateWhileActive(LifeList *cells, int stopPeriod, int matchesNeeded,
                        int genLimit) {
int i, matches;
int gens;

     cellCounts = (int *)realloc(cellCounts, stopPeriod*sizeof(int));

     for (i=0; i<stopPeriod; i++) cellCounts[i]= -1;

     gens=0;
     matches=0;
     while(matches<matchesNeeded && gens<genLimit && cells->ncells>0) {
        generate(cells);

        if (cells->ncells==cellCounts[gens%stopPeriod]) matches++;
        else {
          matches=0;
          cellCounts[gens%stopPeriod]=cells->ncells;
        }

        gens++;
     }

     return gens;

}

