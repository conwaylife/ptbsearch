void initLifeList(LifeList *cells);

void copyLifeList(LifeList *cells1, LifeList *cells2);

void mergeLifeLists(LifeList *cells1, LifeList *cells2, int transl);

void mergeLifeListsMin(LifeList *cells1, LifeList *cells2, int transl);

void intersectLifeLists(LifeList *cells1, LifeList *cells2, int transl);

void removeLifeList(LifeList *cells, LifeList *removecells, int transl);

int matchLifeList(LifeList *cells, LifeList *matchcells, int transl);

int interact(LifeList *pat1, LifeList *pat2, int transl);

void getpat(char *s, LifeList *cells);

void outPostScript(LifeList *cells);
