OscillatorDesc oscillation(LifeList *cells, LifeList *working, int testUpTo);

int simpleSS(LifeList *cells, LifeList *working);

void makeWorkSpace(int n);

initMatchList(PartialOscillatorDesc *osc, int nmatches);

void bestMatches(LifeList *cells1, LifeList *cells2, int period, 
                     int nmatches, PartialOscillatorDesc *osc);

void partialOscillation(LifeList *cells, LifeList *working, 
                        int testUpTo, int testPhases,
                        int nmatches, PartialOscillatorDesc *posc);

/* not a public procedure */
int findTouching(LifeList *cells1, LifeList *cells2);

AlignmentList
   firstTouch(LifeList *cells1, LifeList *cells2, int firstGen, int ngens);

void freeAlignmentList(AlignmentList list);

void dumpAlignments(AlignmentList list);

int emptyNeighbors(LifeList *cells);

int minNeighbors(LifeList *cells);

Transformation normalize(LifeList *cells);

int trail(LifeList *cells, int ngen, Cell *trail, int ntrail, int gen);

void spread(LifeList *cells, int distance);

History makeHistory(LifeList *cells, int ngenerations);

void freeHistory(History hist);

void dumpHistory(History hist);

void getGeneration(LifeList *cells, History hist, int gen);

int mostSurvives(LifeList *pattern, LifeList *perturb, int transl, int steps);

int survives(LifeList *pattern, LifeList *perturb, int transl, int steps,
             int zero_ok);

void setupPerturbors(LifeList *perturbs, Cell *placed, int nplaced,
                    LifeList *justPerturbs, LifeList *reaction);

int placeNewPerturbor(LifeList *seed, LifeList *perturbs,
                       Cell *placed, int nplaced, int newperturb,
                       int initialGen, int finalGen, Cell *aligns,
                       int maxdamage);




