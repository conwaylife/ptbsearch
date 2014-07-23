#include <stdio.h>
#include <stdlib.h>
#include "gen.h"

#ifdef COUNTLOOKUPS
extern int nlookups;
#endif

#define INCY (1<<15)
#define INCX (1)
#define CENTER (1<<14)

extern int neighborhoodTable[];
static int neighborhoodTable2x2[1<<16];
static int bitCount[16];

static int initializedTable = 0;

void initTransitionTable();

/* initializes 4x4 table using 3x3 table */
void init2x2NeighborhoodTable() {

  int i, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, ba, bb, bc, bd, be, bf;
  int ngh0, ngh1, ngh2, ngh3;

  initTransitionTable();

  for (i=0; i<16; i++) {
     bitCount[i] = 
            ( (i>>3) & 1 )
          + ( (i>>2) & 1 )
          + ( (i>>1) & 1 )
          + (  i     & 1 );
  } 


  for (i=0; i<(1<<16); i++) {
     b0=    i  & 1;
     b1=(i>>1) & 1; 
     b2=(i>>2) & 1; 
     b3=(i>>3) & 1; 
     b4=(i>>4) & 1; 
     b5=(i>>5) & 1; 
     b6=(i>>6) & 1; 
     b7=(i>>7) & 1; 
     b8=(i>>8) & 1; 
     b9=(i>>9) & 1; 
     ba=(i>>10) & 1; 
     bb=(i>>11) & 1; 
     bc=(i>>12) & 1; 
     bd=(i>>13) & 1; 
     be=(i>>14) & 1; 
     bf=(i>>15) & 1; 

     ngh3=(bf<<8)+(be<<7)+(b7<<6)+(bd<<5)+(bc<<4)+(b5<<3)+(bb<<2)+(ba<<1)+b3;
     ngh2=(be<<8)+(b7<<7)+(b6<<6)+(bc<<5)+(b5<<4)+(b4<<3)+(ba<<2)+(b3<<1)+b2;
     ngh1=(bd<<8)+(bc<<7)+(b5<<6)+(bb<<5)+(ba<<4)+(b3<<3)+(b9<<2)+(b8<<1)+b1;
     ngh0=(bc<<8)+(b5<<7)+(b4<<6)+(ba<<5)+(b3<<4)+(b2<<3)+(b8<<2)+(b1<<1)+b0;
     
     neighborhoodTable2x2[i]=
             (neighborhoodTable[ngh3]<<3)
          +  (neighborhoodTable[ngh2]<<2)
          +  (neighborhoodTable[ngh1]<<1)
          +  (neighborhoodTable[ngh0]);
/*
         (rules[bc+2*(bf+be+b7+bd+b5+bb+ba+b3)]<<3)
       + (rules[b5+2*(be+b7+b6+bc+b4+ba+b3+b2)]<<2)
       + (rules[ba+2*(bd+bc+b5+bb+b3+b9+b8+b1)]<<1)
       + (rules[b3+2*(bc+b5+b4+ba+b2+b8+b1+b0)]);
*/
  } 

}

/* computes packed bit representation of all non-empty 2x2 neighborhoods
   of 4-bit states  */
int generate2x2(Cell *list, int n, Cell *newlist, int shiftback) {

  int pos;
  int window;
  int i0, i1, inew;

  pos= list[0].position-INCX-INCY;

  i0=1;
  i1=0;

  window= list[0].value;
  inew=0;
  list[n].position=pack(INCY/2, INCY/2);

  while (i0<=n) {
    int nextState = neighborhoodTable2x2[window];

#ifdef COUNTLOOKUPS
    nlookups++;
#endif

    if (nextState) {
       newlist[inew].position= pos + shiftback;
       newlist[inew++].value= nextState;
    }

    /* shift 2x2 window to the right */ 
    window=(window<<8)&0xffff;

    /* if window is empty, jump to the left of next non-empty position */

    if (!window) {

        pos=list[i0].position-2*INCX-INCY;
        if (pos>list[i1].position-2*INCX) pos=list[i1].position-2*INCX;

      }
    
    /* fill right two cells of window with list values */ 

    if (list[i0].position<=pos+2*INCX+INCY) {
      window|=list[i0].value;
      i0++;
    }

    if (list[i1].position<=pos+2*INCX) {
      window|=(list[i1].value<<4);
      i1++;
    }

    /* shift window to the right */
    pos++;

  }

  return inew;
}

/* packs a list of single cells into 2x2 patches*/
int pack2x2(Cell *list, int n, Cell *packedlist) {

  int pos;
  int window;
  int i0, i1, inew;

  pos= list[0].position-INCX-INCY;

  i0=1;
  i1=0;

  window= 1;
  inew=0;
  list[n].position=pack(INCY/2, INCY/2);

  while (i0<=n) {

    if ( ( pos&(INCY|INCX) ) == 0  ) {
     int x,y;

       unpack(pos, &x, &y);
       packedlist[inew].position= pack(x/2,y/2);
       packedlist[inew].value= window;

       unpack(packedlist[inew].position, &x, &y);
      
       inew++; 
    }

    /* shift 2x2 window to the right */ 
    window=(window<<1)&0xa;

    /* if window is empty, jump to the left of next non-empty position */

    if (!window) {

        pos=list[i0].position-2*INCX-INCY;
        if (pos>list[i1].position-2*INCX) pos=list[i1].position-2*INCX;

      }
    
    /* fill right two cells of window with list values */ 

    if (list[i0].position<=pos+2*INCX+INCY) {
      window|=1;
      i0++;
    }

    if (list[i1].position<=pos+2*INCX) {
      window|=(1<<2);
      i1++;
    }

    /* shift window to the right */
    pos++;

  }

  return inew;
}

/* unpacks 2x2 patches into a cell list */
int unpack2x2(Cell *list, int n, Cell *unpackedlist) {

int i, j, inew, pos;

   list[n].position=pack(INCY/2, INCY/2);

   i=0;
   inew = 0;

   while (i<n) {

      int x, y;

      pos=list[i].position;
      unpack(pos, &x, &y);
      pos=pack(-INCY/2,y);

      j=i;
      while (list[j].position<pos+INCY) {

         unpack(list[j].position, &x, &y);

         if (list[j].value & 8) { 
             unpackedlist[inew].value=1; 
             unpackedlist[inew++].position=pack(2*x, 2*y);
         } 
         if (list[j].value & 4) { 
             unpackedlist[inew].value=1; 
             unpackedlist[inew++].position=pack(2*x+1, 2*y);
         }

         j++;
      }

      j=i;
      while (list[j].position<pos+INCY) {

         unpack(list[j].position, &x, &y);

         if (list[j].value & 2) {
             unpackedlist[inew].value=1; 
             unpackedlist[inew++].position=pack(2*x, 2*y+1);
         }
         if (list[j].value & 1) {
             unpackedlist[inew].value=1; 
             unpackedlist[inew++].position=pack(2*x+1, 2*y+1);
         }

         j++;
      }
    
      i=j; 

   }

   return inew;

}

void packedGenerateTwoSteps(LifeList *cells) {

      resizeIfNeeded(cells, 4*cells->ncells);
      cells->ncells=generate2x2(cells->neighborhoods, cells->ncells, 
                                cells->cellList, 0);

      resizeIfNeeded(cells, 4*cells->ncells);
      cells->ncells=generate2x2(cells->cellList, cells->ncells, 
                                cells->neighborhoods, INCX+INCY);

}


void packedGenerate(LifeList *cells, int ngens) {
int i;
int nextshift;
int steps, parity;

   parity=ngens%2; 
   steps=ngens/2; 

   if (!initializedTable) {
     init2x2NeighborhoodTable();
     initializedTable= 1;
   }

   cells->ncells=pack2x2(cells->cellList, cells->ncells, cells->neighborhoods);

   for (i=1; i<=steps; i++) packedGenerateTwoSteps(cells);

   cells->ncells=unpack2x2(cells->neighborhoods, cells->ncells, 
                           cells->cellList);

   if (parity) {
     generate(cells);
   }

}

static int *cellCounts=NULL;

int packedGenerateWhileActive(LifeList *cells, int stopPeriod, 
                              int matchesNeeded, int genLimit) {
int i, matches;
int gens;

     if (!initializedTable) {
       init2x2NeighborhoodTable();
       initializedTable= 1;
     }


     cellCounts = (int *)realloc(cellCounts, stopPeriod*sizeof(int));

     for (i=0; i<stopPeriod; i++) cellCounts[i]= -1;

     cells->ncells=pack2x2(cells->cellList, cells->ncells, 
                           cells->neighborhoods);

     gens=0;
     matches=0;
     while(matches<matchesNeeded && 2*gens<genLimit && cells->ncells>0) {
        packedGenerateTwoSteps(cells);

        if (cells->ncells==cellCounts[gens%stopPeriod]) matches++;
        else {
          matches=0;
          cellCounts[gens%stopPeriod]=cells->ncells;
        }

        gens++;
     }

     cells->ncells=unpack2x2(cells->neighborhoods, cells->ncells, 
                             cells->cellList);

     return 2*gens;

}

