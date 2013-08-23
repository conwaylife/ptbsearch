void resizeIfNeeded(LifeList *cells, int n);

int nextGen(Cell *counts, int ncounts, Cell *cellList);

int nextGenFromTable(Cell *neighborhoods, int nneighborhoods, Cell *cellList);

void makeRowMajor(LifeList *cells);

void oscCheck(int n);

void initNeighborhoodTable();

void generate(LifeList *cells);

void initPopHistory();

void longestOscillation(int *period, int *repetition);
