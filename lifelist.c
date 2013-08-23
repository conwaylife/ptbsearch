/* All code copyright Paul Callahan 1996  (callahan@inf.ethz.ch) */

#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>
#include <assert.h>


void initLifeList(LifeList *cells) {

   cells->ncells=0;
   cells->maxsize=0;

   resizeIfNeeded(cells, 10);

}

void copyLifeList(LifeList *cells1, LifeList *cells2) {

   resizeIfNeeded(cells2, cells1->ncells); 
   copyList(cells1->cellList, cells1->ncells, cells2->cellList, 0);
   cells2->ncells=cells1->ncells; 

}

void getLifeListValues(LifeList *cells, LifeList *values) {

/* values must be a superset of cells or a bus error could occur */

    getValues(cells->cellList, cells->ncells, values->cellList);

}

void mergeLifeLists(LifeList *cells1, LifeList *cells2, int transl) {

/* note: destroys neighborhood values */

    copyList(cells2->cellList, cells2->ncells, cells2->neighborhoods, transl);

    resizeIfNeeded(cells1, cells1->ncells+cells2->ncells);

    cells1->ncells=combineLists(cells1->cellList, cells1->ncells,
                                cells2->neighborhoods, cells2->ncells,
                                cells1->neighborhoods);

    copyList(cells1->neighborhoods, cells1->ncells, cells1->cellList, 0);
}

void mergeLifeListsMin(LifeList *cells1, LifeList *cells2, int transl) {

/* note: destroys neighborhood values */

    copyList(cells2->cellList, cells2->ncells, cells2->neighborhoods, transl);

    resizeIfNeeded(cells1, cells1->ncells+cells2->ncells);

    cells1->ncells=combineListsMin(cells1->cellList, cells1->ncells,
                                   cells2->neighborhoods, cells2->ncells,
                                   cells1->neighborhoods);

    copyList(cells1->neighborhoods, cells1->ncells, cells1->cellList, 0);
}

void intersectLifeLists(LifeList *cells1, LifeList *cells2, int transl) {

/* note: destroys neighborhood values */

    copyList(cells2->cellList, cells2->ncells, cells2->neighborhoods, transl);

    cells1->ncells=intersectLists(cells1->cellList, cells1->ncells,
                                  cells2->neighborhoods, cells2->ncells,
                                  cells1->neighborhoods);

    copyList(cells1->neighborhoods, cells1->ncells, cells1->cellList, 0);
}

void removeLifeList(LifeList *cells, LifeList *removecells, int transl) {

/* note: destroys removecells neighborhood values */

    copyList(removecells->cellList, removecells->ncells, 
             removecells->neighborhoods, transl);
        
    cells->ncells=subtractLists(removecells->neighborhoods, 
                                removecells->ncells,
                                cells->cellList, cells->ncells);  
}

int matchLifeList(LifeList *cells, LifeList *matchcells, int transl) {
int matches;
/* note: destroys matchcells neighborhood values */

    copyList(matchcells->cellList, matchcells->ncells, 
             matchcells->neighborhoods, transl);

    matches=countMatch(matchcells->neighborhoods, matchcells->ncells,
                       cells->cellList, cells->ncells);  

    return matches;
}

static LifeList both;

int interact(LifeList *pat1, LifeList *pat2, int transl) {

/* both pat1 and pat2 are generated one step */

   copyLifeList(pat1, &both);
   mergeLifeLists(&both, pat2, transl); 

   generate(pat1);
   generate(pat2);
   generate(&both);

   return !( both.ncells == pat1->ncells + pat2->ncells &&
             pat1->ncells == countMatch(pat1->cellList, pat1->ncells,
                                        both.cellList, both.ncells) &&
             pat2->ncells == matchLifeList(&both, pat2, transl) ); 

}


void getpat(char *s, LifeList *cells) {
int curx, cury;

     curx=cury=0;

     cells->ncells=0;

     while (*s) {
        if ((*s)=='*' || ( (*s)>='A' && (*s)<='Z' ) ||
                         ( (*s)>='a' && (*s)<='z' )  ) {
            resizeIfNeeded(cells, cells->ncells+1);
            cells->cellList[cells->ncells].position=pack(curx, cury);
            cells->cellList[cells->ncells].value=
                  ( (*s) == '*' ) ? 1 : (*s) ;
            cells->ncells++;
            curx++;
        } else if ((*s)=='!') {
            curx=0;
            cury++;
        } else {
            curx++;
        }
        s++;
     }
}


void outPostScript(LifeList *cells, float cellsize, int spacing) {

   int i;
   int x, y;
   int extra;
   BoundingBox bb;

   bb=makeBoundingBox(cells->cellList, cells->ncells); 

   if (spacing>0) {
      extra = (spacing-(bb.ux-bb.lx+1)%spacing)%spacing;
      bb.lx-=(extra/2+extra%2);
      bb.ux+=extra/2;
   
      extra = (spacing-(bb.uy-bb.ly+1)%spacing)%spacing;
      bb.ly-=(extra/2+extra%2);
      bb.uy+=extra/2;
   }

   printf("%%!PS-Adobe-2.0 EPSF-1.2\n");
   printf("%%%%DocumentFonts: Times-Bold\n");
   printf("%%%%Pages: 1\n");
   printf("%%%%BoundingBox: 0 0 %d %d\n", (int) ( cellsize*(float)(bb.ux-bb.lx)
                                                  +2*cellsize + 0.99), 
                                          (int) ( cellsize*(float)(bb.uy-bb.ly)
                                                  +2*cellsize + 0.99) ); 
   printf("%%%%EndComments\n");                
   printf("0.3 setlinewidth\n");
   printf("/cell {%f mul exch %f mul exch %f 0 360 arc fill} def\n", 
          cellsize, cellsize, 0.3*cellsize);

   if (spacing>0) {
     for (x= -1; x<=bb.ux-bb.lx; x+=spacing) 
        printf("%f %f moveto %f %f lineto stroke\n",
                  cellsize*x+1.5*cellsize, 0.5*cellsize, 
                  cellsize*x+1.5*cellsize, cellsize*(bb.uy-bb.ly)+1.5*cellsize);
  
     for (y= -1; y<=bb.uy-bb.ly; y+=spacing) 
        printf("%f %f moveto %f %f lineto stroke\n",
                  0.5*cellsize, cellsize*y+1.5*cellsize, 
                  cellsize*(bb.ux-bb.lx)+1.5*cellsize, cellsize*y+1.5*cellsize);
   } 

   for (i=0; i<cells->ncells; i++) {

      unpack(cells->cellList[i].position, &x, &y);

      printf("%d %d cell\n", (x-bb.lx)+1, (bb.uy-y)+1);
   }
}

void outFig(LifeList *cells, int cellboxlen) {
/* Outputs pattern as an Xfig compound object. */

   int i;
   int x, y;
   int extra;
   int cellrad;
   int xlen, ylen;
   int border=3;

   BoundingBox bb;

   cellrad=cellboxlen/3; 

   printf("#FIG 3.1\n");
   printf("Landscape\n");
   printf("Center\n");
   printf("Inches\n");
   printf("1200 2\n");

   bb=makeBoundingBox(cells->cellList, cells->ncells); 

   xlen = (bb.ux-bb.lx+1+2*border)*cellboxlen;
   ylen = (bb.uy-bb.ly+1+2*border)*cellboxlen;

   printf("6 0 0 %d %d\n", xlen, ylen);

   for (i=0; i<cells->ncells; i++) {
      int figx, figy;

      unpack(cells->cellList[i].position, &x, &y);
      figx=(x-bb.lx+border)*cellboxlen+cellboxlen/2;
      figy=(y-bb.ly+border)*cellboxlen+cellboxlen/2;

      printf("1 3 0 1 -1 7 0 0 0 0.000 1 0.0000 ");
      printf("%d %d %d %d %d %d %d %d\n", figx, figy,
                                        cellrad, cellrad,
                                        figx, figy,
                                        figx+cellrad, figy+cellrad);
   }
      printf("2 2 2 1 -1 7 0 0 -1 3.000 0 0 -1 0 0 5\n");
      printf("\t%d %d %d %d %d %d %d %d %d %d\n",     0,   0, xlen, 0,
                                                xlen, ylen,     0, ylen, 0, 0); 
   printf("-6\n");
}


