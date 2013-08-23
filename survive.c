#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>

main(int argc, char *argv[]) {

  static LifeList cells;
  static LifeList matchcells;
  static LifeList outcells;
  int i,j,n=0;
  int gens=500;
  int x,y;
  int match;
  int pos;
  int count[240];
  int least= 0;
  int fail;
  int damaged;
  int restored;
  int prodcells; 
  char outpat[100000];


  History hist;
  int period, repetitions;
  Cell lifetrail[5000];
  static char nextpat[2000];
  int ncls;

  initLifeList(&cells);
  initLifeList(&matchcells);
  initLifeList(&outcells);

  while(gets(nextpat)) {

    for (i=0; i<240; i++) { count[i] = i; }

    getpat(nextpat, &cells);
    copyLifeList(&cells, &matchcells);

    if (argc>1) {
      for (i=0; argv[1][i]; i++) { 
	matchcells.ncells = 
	  removeIfEquals(matchcells.cellList, matchcells.ncells, argv[1][i]); 
      }
    }

    fail=0;
    damaged=0;
    restored=0;
    for (i=0; i<gens; i++) {
      generate(&cells); 

      //      printf("%d %d %d %d\n", i, fail, damaged, restored);

      count[i%240] = cells.ncells;
   
      if (cells.ncells == 0) break;
   
      match = 1;
      for (j=0; j<180; j++) {
	if (count[j] != count[j+60]) {
	  match = 0;
	  break;
	} 
      }
   
      if (match) break;
   
      if ( matchLifeList(&cells, &matchcells, 0) < matchcells.ncells) {
	fail++; 
	restored=0;
      } else {
	prodcells = cells.ncells - matchcells.ncells;
        copyLifeList(&cells, &outcells);

	restored++;
	if (restored>10) {
	  if (fail>damaged) damaged=fail;
	  fail = 0;
	}
      }
   
      if (fail>150) break;

    }
   
    if (damaged) {
      fflush(stdout);
      removeLifeList(&outcells, &matchcells, 0);
      makeString(outcells.cellList, outcells.ncells, outpat);

      printf("%s %d %d %d %d %s\n", nextpat, damaged, fail, i-fail, prodcells, outpat);
    }
   
  }
}

