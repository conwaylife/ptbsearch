/* All code copyright Paul Callahan 1996-1997 */

#define INCY (1<<15)
#define INCX (1)
#define CENTER (1<<14)
#define POPHISTORYSIZE 6

#define ERASE  0x0000
#define RED    0x0001
#define GREEN  0x0010
#define BLUE   0x0100
#define YELLOW 0x1000
#define DEFAULT  -1
#define CYAN -2

#define NOTRANS 0
#define TRANSON 1
#define TRANSOFF 2


#define SCROLLREGIONFRACT 6

#define MAXMISSES 5
#define INITMAXSIZE 100

#define SPACE 80
#define MAXCOL 400
#define MAXCLUSTERS 5000

/*
   Cell:
   Bit-packed position and value of grid cell
   (usually, but not always, representing a Life cell) 
*/
typedef struct {
   int position;
   unsigned int value;
} Cell;



/*
   LifeList:
     Structure consisting of a row-major-ordered (rmo) list of cells 
     and another of the cells neighborhoods used while computing a
     Life generation. Note: maxsize is the maximum memory currently 
     allocated.
      
*/
typedef struct {
    Cell *cellList;
    Cell *neighborhoods;
    int ncells, maxsize;
  } LifeList;


/*
    Transformation:
      Representation of a reorientation and translation 
      of a Life pattern.

*/
typedef struct {

   int flipxF, flipyF, transposeF;
   int translateBy;

} Transformation;

/*
   BoundingBox:
     Bounding box of a list of cells (usually a Life pattern).

*/
typedef struct {

   int lx, ux;
   int ly, uy;

} BoundingBox;

/*
  OscillatorDesc:
   Result of oscillator test, including period, transformation,
   and maximum period tested.
*/
typedef struct {

   int period;
   int testedUpTo;
   Transformation T;

} OscillatorDesc;

/* 
  ClusterDesc:
    Description of a cluster, including its oscillation, if any
    and its bounding box. */
typedef struct {
    OscillatorDesc osc;
    BoundingBox bb;
} ClusterDesc;

/*
   PartialOscillatorDesc: 
     Result of "almost oscillator" test. 
*/
typedef struct {

   int period;
   int matchDistance;
   Transformation T;

} PartialOscillatorDesc;

/*
    History:
       Sequence of rmo lists representing the history of a Life
       pattern, returned by makeHistory.
*/
typedef struct {

   Cell **cellList;
   int *ncells;
   int ngenerations;

   Cell *trail;
   int ntrail;

} History;


/*
    AlignmentList:
       Sequence of Alignments, representing the relative position of
       two pairs at the generations at which they first touch,
       returned by FirstTouch.
*/
typedef struct {

   Cell **alignments;
   int *nalignments;

   int firstGen;
   int ngens;
} AlignmentList;



int combineLists(Cell *list1, int n1, Cell *list2, int n2, Cell *list3);

int subtractLists(Cell *list1, int n1, Cell *list2, int n2);

int combineListsMin(Cell *list1, int n1, Cell *list2, int n2, Cell *list3);

int countMatch(Cell *list1, int n1, Cell *list2, int n2);

int sumColumnNeighbors(Cell *list, int n, Cell *newlist);

void extractCenterCells(Cell *list, int n, Cell *extractFrom);

int nextGen(Cell *counts, int ncounts, Cell *cellList);

int sumAllNeighbors(Cell *list, int n, Cell *newlist);

int findNeighborhoods(Cell *list, int n, Cell *newlist);

int countGreaterThan(Cell *list, int n, int value);

int removeGreaterThan(Cell *list, int n, int value);

int removeLessThan(Cell *list, int n, int value);

int removeIfZero(Cell *list, int n, int value);

int getQuadrantCells(Cell *fromList, int n, Cell *toList, int origin,
                                           int dx, int dy);

BoundingBox makeBoundingBox(Cell *list, int n);

void makeString(Cell *list, int n, char *patstring);

void setValues(Cell *list, int n, int value);

void transformBack(Cell *list, Transformation T, int n);

void transform(Cell *list, Transformation T, int n);

int copyList(Cell *fromList, int n, Cell *toList, int translateBy);

int condCopyList(Cell *fromList, int n, Cell *toList, int value);

int convolve(Cell *list1, int n1, Cell *list2, int n2,
             Cell **convolution, Cell **scratch1, Cell **scratch2,
             void (*morespace) (int));

int convolveMin(Cell *list1, int n1, Cell *list2, int n2,
                Cell *convolution, Cell *scratch1, Cell *scratch2);

void sortRowMajor(Cell *list, int n);

int pack(int x, int y);
int packtrans(int x, int y);

void unpack(int packed, int *x, int *y);

void unpacktrans(int packed, int *x, int *y);

void transpose(Cell *list, int n);


void flipx(Cell *list, int n);

void flipy(Cell *list, int n);

int getValues(Cell *cellList, int n, Cell *valueList);

int pruneList(Cell *keepCells, int n, Cell *inList);

void dumpCells(Cell *list, int n);

void dumpTransCells(Cell *list, int n);

int firstZero(Cell *list, int n);

int compare(Cell *list1, Cell *list2, int n);

void generate(LifeList *cells);

void packedGenerate(LifeList *cells, int ngens);

void resizeIfNeeded(LifeList *cells, int n);

void freeLifeList(LifeList *cells);

int sumNeighbors(LifeList *cells);

int minNeighbors(LifeList *cells);

void makeRowMajor(LifeList *cells);

int findComponents(LifeList *cells, LifeList *labeled, int distance);

int independentUpTo(LifeList *cells, LifeList *cumulative,
                     LifeList *working, int ngens); 

int getClusterDesc(LifeList *cells, LifeList *working1,
                   LifeList *working2, int ngens,
                   LifeList *clusters,
                   ClusterDesc *clusterInfo);

Transformation normalize(LifeList *cells);

OscillatorDesc oscillation(LifeList *cells, LifeList *working, int testUpTo);

OscillatorDesc simpleOscillation(LifeList *cells, LifeList *working, 
                                 int testUpTo);

int simpleSS(LifeList *cells, LifeList *working);

void partialOscillation(LifeList *cells, LifeList *working,
                        int testUpTo, int testPhases,
                        int nmatches, PartialOscillatorDesc *posc);

History makeHistory(LifeList *cells, int ngenerations);

void freeHistory(History hist);

void dumpHistory(History hist);

void getGeneration(LifeList *cells, History hist, int gen);

void initPopHistory();

void longestOscillation(int *period, int *repetition);

void readCellsColor(LifeList *cells, char *patname, int color);

AlignmentList
    firstTouch(LifeList *cells1, LifeList *cells2, int firstGen, int ngens);

void freeAlignmentList(AlignmentList list);

void dumpAlignments(AlignmentList list);

void generateColor(LifeList *cells);

void spreadColors(LifeList *cells);

void initColorTables();

int emptyNeighbors(LifeList *cells);

int trail(LifeList *cells, int ngen, Cell *trail, int ntrail, int gen);

void spread(LifeList *cells, int distance);

void mergeLifeLists(LifeList *cells1, LifeList *cells2, int transl);

void mergeLifeListsMin(LifeList *cells1, LifeList *cells2, int transl);

void removeLifeList(LifeList *cells, LifeList *removecells, int transl);

int matchLifeList(LifeList *cells, LifeList *matchcells, int transl);

int interact(LifeList *pat1, LifeList *pat2, int transl);

void initLifeList(LifeList *cells); 

void copyLifeList(LifeList *cells1, LifeList *cells2); 

void getLifeListValues(LifeList *cells, LifeList *values); 

void *myrealloc(void *pointer, size_t size);

void logcomment(char *comment);

void getpat(char *s, LifeList *cells); 

int placeNewPerturbor(LifeList *seed, LifeList *perturbs,
                       Cell *placed, int nplaced, int newperturb,
                       int initialGen, int finalGen, Cell *aligns,
                       int maxdamage);

int readPatList(LifeList *pat, char *patname);

int survives(LifeList *pattern, LifeList *perturb, int transl, int steps);

void setupPerturbors(LifeList *perturbs, Cell *placed, int nplaced,
                     LifeList *justPerturbs, LifeList *reaction);

void perturbEnum(LifeList *seed, LifeList *perturbs, int nperturbs,
           int depth, int maxDepth, int mingen, int maxgen, int maxvanish);

void cumulativeImage(LifeList *cells, LifeList *cumulative, int ngens);

int approaching(ClusterDesc cluster1, ClusterDesc cluster2);

int possibleInteraction(ClusterDesc *clusters, int nClusters);

int generateWhileActive(LifeList *cells, int stopPeriod, int matchesNeeded,
                        int genLimit); 

int packedGenerateWhileActive(LifeList *cells, int stopPeriod, 
                              int matchesNeeded, int genLimit); 

int quiescent(LifeList *cells, int stopPeriod, int matchesNeeded,
              int retryAfter, int genLimit, int *ngens, 
              int oscPeriod, int clusterPeriod);

void countMoving(int *gliders, int *spaceships);

void outPostScript(LifeList *cells, float cellsize, int spacing);
