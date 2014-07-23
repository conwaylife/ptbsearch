#include <stdio.h>
#include <limits.h>
#include "gen.h"

int initLabels(Cell *cells, int n) {

   int i;

   for (i=0; i<n; i++) {
       cells[i].value=i;
   }

   return n;
}


int relabel(Cell *cells, int n) {

  int i, label;

  label=0;

  for (i=0; i<n; i++) {

     if (cells[i].value == i) {
           cells[i].value = label++;
     } else {
           cells[i].value = cells[cells[i].value].value;
     }

  }

  return label;

}


int findComponents(LifeList *cells, LifeList *labeled, int distance) {
  int noChange;
  int nSpread;
  int nComponents;

    resizeIfNeeded(labeled, cells->ncells);
    labeled->ncells= copyList(cells->cellList, cells->ncells, 
                               labeled->cellList, 0);

    labeled->ncells= initLabels(labeled->cellList, cells->ncells);

    do {
       int k;
       int n;

       for (k=0; k<distance; k++) {
          n= minNeighbors(labeled);
          labeled->ncells=
              copyList(labeled->neighborhoods, n, labeled->cellList, 0);
       }

       labeled->ncells= pruneList(cells->cellList, cells->ncells, 
                           labeled->cellList);
       
       noChange=getValues(cells->cellList, cells->ncells, labeled->cellList);
    } while (!noChange);

    nComponents =relabel(labeled->cellList, labeled->ncells);

    return nComponents;
}

