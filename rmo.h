int combineLists(Cell *list1, int n1, Cell *list2, int n2, Cell *list3);

int intersectLists(Cell *list1, int n1, Cell *list2, int n2, Cell *list3);

int subtractLists(Cell *list1, int n1, Cell *list2, int n2);

int countMatch(Cell *list1, int n1, Cell *list2, int n2);

int sumColumnNeighbors(Cell *list, int n, Cell *newlist);

int sumAllNeighbors(Cell *list, int n, Cell *newlist);

int findNeighborhoods(Cell *list, int n, Cell *newlist);

int combineListsMin(Cell *list1, int n1, Cell *list2, int n2, Cell *list3) ;

void extractCenterCells(Cell *list, int n, Cell *extractFrom);

int getValues(Cell *cellList, int n, Cell *valueList);

int pruneList(Cell *keepCells, int n, Cell *inList);

int uniqCells(Cell *list, int n);

int copyList(Cell *fromList, int n, Cell *toList, int translateBy);

void setValues(Cell *list, int n, int value);

int removeGreaterThan(Cell *list, int n, int value);

int removeIfEquals(Cell *list, int n, int value);

int removeLessThan(Cell *list, int n, int value);

int removeIfZero(Cell *list, int n, int value);

int condCopyList(Cell *fromList, int n, Cell *toList, int value);

int getQuadrantCells(Cell *fromList, int n, Cell *toList, int origin,
                                           int dx, int dy);

int convolve(Cell *list1, int n1, Cell *list2, int n2, 
             Cell *convolution, Cell *scratch1, Cell *scratch2);

int convolveMin(Cell *list1, int n1, Cell *list2, int n2, 
                Cell *convolution, Cell *scratch1, Cell *scratch2);

void quickSortCells(Cell list[], int left, int right);

void sortRowMajor(Cell *list, int n);

int pack(int x, int y);

int packtrans(int x, int y);

void unpack(int packed, int *x, int *y);

void unpacktrans(int packed, int *tx, int *ty);

void transpose(Cell *list, int n);

void flipx(Cell *list, int n);

void flipy(Cell *list, int n);

void dumpCellBits(Cell *list, int n);

void dumpCellHex(Cell *list, int n);

void dumpCells(Cell *list, int n);

void dumpTransCells(Cell *list, int n);

int firstZero(Cell *list, int n);

int compare(Cell *list1, Cell *list2, int n);

BoundingBox makeBoundingBox(Cell *list, int n) ;

void makeString(Cell *list, int n, char *patstring);

void transformBack(Cell *list, Transformation T, int n);

void transform(Cell *list, Transformation T, int n);
