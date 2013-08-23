#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>
#include <assert.h>

#define MAXDEPTH 10

static char out[8000];

Cell perturbPlace[MAXDEPTH];
int perturbGen[MAXDEPTH];

static LifeList reaction[MAXDEPTH], justPerturbs;

static Cell alignstore[100000];
static Cell *aligns=alignstore;

static LifeList tmp;

int ptbPrecedes(int gen1, int ptb1, int pos1,
                int gen2, int ptb2, int pos2) {

    if (gen1<gen2) return 1;
    if (gen1>gen2) return 0;

    if (ptb1<ptb2) return 1;
    if (ptb1>ptb2) return 0;

    if (pos1<pos2) return 1;

    return 0;
}

void outTr(int transl) {
int x,y;

   unpacktrans(transl, &x, &y);
   printf("  (%3d,%3d) ", x,y);
}

void perturbEnum(LifeList *seed, LifeList *perturbs, int nperturbs,
           int depth, int maxDepth, int mingen, int maxgen, int maxvanish) {
int i, naligns;
int iperturb, thisgen;


   for (iperturb=0; iperturb<nperturbs; iperturb++) {

     copyLifeList(seed, reaction+depth);
     setValues(reaction[depth].cellList, reaction[depth].ncells, 1); 

     naligns=placeNewPerturbor(reaction+depth, perturbs, 
                               perturbPlace, depth, iperturb,
                               mingen, maxgen, aligns, 5);

     setupPerturbors(perturbs, perturbPlace, depth, 
                     &justPerturbs, reaction+depth);


     for (thisgen=0; thisgen<mingen; thisgen++) generate(reaction+depth);

     for (; thisgen<maxgen; thisgen++) {
       for (i=0; i<naligns; i++) {
         int j;
         int tvanish = maxvanish;

         int oldcount;
         assert((aligns-alignstore)+i < 100000);
	 if (aligns[i].value==thisgen &&
	     (depth<=0 ||
	      ptbPrecedes(perturbGen[depth-1], 
			  perturbPlace[depth-1].value, 
			  perturbPlace[depth-1].position,
			  thisgen, iperturb, aligns[i].position)
	      )
	     ) {
	   copyLifeList(reaction+depth, &tmp);

           justPerturbs.ncells=0;
           for (j=0; j<depth; j++) {
               mergeLifeLists(&justPerturbs, perturbs+perturbPlace[j].value, 
                                             perturbPlace[j].position);
           }

	   mergeLifeLists(&tmp, perturbs+iperturb, aligns[i].position);

           oldcount=justPerturbs.ncells;
	   mergeLifeLists(&justPerturbs, perturbs+iperturb, 
			                      aligns[i].position);

	   // if (iperturb==8) printf("chose a block %d %d\n", perturbs[iperturb].ncells, countGreaterThan(perturbs[iperturb].cellList, perturbs[iperturb].ncells, 1)  );

	   if ( countGreaterThan(perturbs[iperturb].cellList,
                                perturbs[iperturb].ncells, 1) ==
                                perturbs[iperturb].ncells) tvanish--;

           if (tvanish>=0 &&
             oldcount+perturbs[iperturb].ncells == justPerturbs.ncells &&
               survives(&tmp, &justPerturbs, 0, 15)) {

	     if (depth >= maxDepth) {
               if (restored(&tmp, &justPerturbs, 0, 50)) {
	       copyLifeList(seed, &tmp);
	       mergeLifeLists(&tmp, &justPerturbs, 0);
	       makeString(tmp.cellList, tmp.ncells, out);
//               outTr(perturbPlace[0].position);
//               outTr(perturbPlace[1].position);
//               outTr(aligns[i].position);
	       printf("%s %d\n", out, thisgen);
/*
               printf("%d %d %d ", perturbPlace[0].position, 
                                  perturbPlace[0].value,
                                  perturbGen[0]);
               printf("%d %d %d\n", aligns[i].position, 
                                  iperturb,
                                  aligns[i].value);
*/
               fflush(stdout);
               }
	     } else {
	       perturbPlace[depth].position= aligns[i].position;
	       perturbPlace[depth].value= iperturb;
	       perturbGen[depth]= thisgen;
               aligns+=naligns;
	       perturbEnum(seed, perturbs, nperturbs, depth+1, maxDepth,
			   thisgen, maxgen, tvanish);  
               aligns-=naligns;
	     } 
	   }
	 }
       }
       generate(reaction+depth);
     }
   }
}
