/* All code copyright Paul Callahan 1996-1998  (callahan@inf.ethz.ch) */

#include <stdio.h>
#include <limits.h>
#include "gen.h"
#include <malloc.h>
#include <assert.h>

int combineLists(Cell *list1, int n1, Cell *list2, int n2, Cell *list3) {

    int i,j,n3;

    i=0; j=0; n3=0;

    list1[n1].position= INT_MAX;
    list2[n2].position= INT_MAX;
/*
    printf("=======\n");
*/

    while (i<n1 || j<n2) {

       if (list1[i].position == list2[j].position) {
         list3[n3].position=list1[i].position;
         list3[n3++].value = list1[i++].value + list2[j++].value;
       } else if (list1[i].position < list2[j].position) {
         list3[n3++] = list1[i++];
       } else {
         list3[n3++] = list2[j++];
       }
/*
       printf("! %x %x %d %d %x %d %d !\n", 
              list1[i-1].position, list2[j-1].position, i-1, j-1, 
              list3[n3-1].position,n3-1, list3[n3-1].value);
*/
     }

    return n3;
  }

int intersectLists(Cell *list1, int n1, Cell *list2, int n2, Cell *list3) {

    int i,j,n3;

    i=0; j=0; n3=0;

    list1[n1].position= INT_MAX;
    list2[n2].position= INT_MAX;

    while (i<n1 || j<n2) {

       if (list1[i].position == list2[j].position) {
         list3[n3].position=list1[i].position;
         list3[n3++].value = list1[i++].value + list2[j++].value;
       } else if (list1[i].position < list2[j].position) {
         i++;
       } else {
         j++;
       }

     }

    return n3;
  }

int subtractLists(Cell *list1, int n1, Cell *list2, int n2) {

    int i,j,n3;

    i=0; j=0; n3=0;

    list1[n1].position= INT_MAX;
    list2[n2].position= INT_MAX;

    while (i<n1 || j<n2) {

       if (list1[i].position == list2[j].position) {
         i++;
         j++;
       } else if (list1[i].position < list2[j].position) {
         i++;
       } else {
         list2[n3++]= list2[j++];
       }

     }

    return n3;
  }

int countMatch(Cell *list1, int n1, Cell *list2, int n2) {

    int i,j,matches;

    i=0; j=0; matches=0;

    list1[n1].position= INT_MAX;
    list2[n2].position= INT_MAX;

    while (i<n1 && j<n2) {

       if (list1[i].position == list2[j].position) {
         i++;
         j++;
         matches++;
       } else if (list1[i].position < list2[j].position) {
         i++;
       } else {
         j++;
       }

     }

    return matches;
  }




int sumColumnNeighbors(Cell *list, int n, Cell *newlist) {

  int wShape;
  int wSum;
  int pos;

  int i, inew;

  pos= list[0].position-1;
  wSum= list[0].value;

  i=1;
  wShape=1;
  inew=0;
  list[n].position=INT_MAX;

  while (i<=n) {

    newlist[inew].position= pos;
    newlist[inew++].value= wSum;

    switch(wShape) {

    case 1: wShape=2; break; 
    case 2: wShape=4; break;
    case 3: wShape=6; break;
    case 4: wShape=0; wSum-=list[i-1].value; 
                      pos=list[i].position-2;  break;
    case 5: wShape=2; wSum-=list[i-2].value;   break;
    case 6: wShape=4; wSum-=list[i-2].value;   break;
    case 7: wShape=6; wSum-=list[i-3].value;   break;  

    }

    if (list[i].position<=pos+2) {
      wShape++;
      wSum+=list[i++].value;
    }

    pos++;
  }

  return inew;
}

int sumAllNeighbors(Cell *list, int n, Cell *newlist) {

  int wShape0, wShape1, wShape2;
  int wSum;
  int pos;

  int i0, i1, i2, inew;

  if (n==0) return 0;

  pos= list[0].position-INCX-INCY;
  wSum= list[0].value;

  i0=1;
  i1=0;
  i2=0;
  wShape0=1; wShape1=0; wShape2=0;
  inew=0;
  list[n].position=pack(INCY/2, INCY/2);

  while (i0<=n) {
    newlist[inew].position= pos;
    newlist[inew++].value= wSum;

    switch(wShape0) {

    case 1: wShape0=2; break; 
    case 2: wShape0=4; break;
    case 3: wShape0=6; break;
    case 4: wShape0=0; wSum-=list[i0-1].value;   break;
    case 5: wShape0=2; wSum-=list[i0-2].value;   break;
    case 6: wShape0=4; wSum-=list[i0-2].value;   break;
    case 7: wShape0=6; wSum-=list[i0-3].value;   break;  

    }

    switch(wShape1) {

    case 1: wShape1=2; break; 
    case 2: wShape1=4; break;
    case 3: wShape1=6; break;
    case 4: wShape1=0; wSum-=list[i1-1].value;   break;
    case 5: wShape1=2; wSum-=list[i1-2].value;   break;
    case 6: wShape1=4; wSum-=list[i1-2].value;   break;
    case 7: wShape1=6; wSum-=list[i1-3].value;   break;  

    }

    switch(wShape2) {

    case 1: wShape2=2; break; 
    case 2: wShape2=4; break;
    case 3: wShape2=6; break;
    case 4: wShape2=0; wSum-=list[i2-1].value;   break;
    case 5: wShape2=2; wSum-=list[i2-2].value;   break;
    case 6: wShape2=4; wSum-=list[i2-2].value;   break;
    case 7: wShape2=6; wSum-=list[i2-3].value;   break;  

    }

    if (!(wShape0 | wShape1 | wShape2)) {
        pos=list[i0].position-2-INCY;
        if (pos>list[i1].position-2) pos=list[i1].position-2;
        if (pos>list[i2].position-2+INCY) pos=list[i2].position-2+INCY;
      }

    if (list[i0].position<=pos+2+INCY) {
      wShape0++;
      wSum+=list[i0++].value;
    }

    if (list[i1].position<=pos+2) {
      wShape1++;
      wSum+=list[i1++].value;
    }

    if (list[i2].position<=pos+2-INCY) {
      wShape2++;
      wSum+=list[i2++].value;
    }


    pos++;

  }

  return inew;
}


int findNeighborhoods(Cell *list, int n, Cell *newlist) {

  int pos;
  int window;
  int i0, i1, i2, inew;

  pos= list[0].position-INCX-INCY;

  i0=1;
  i1=0;
  i2=0;
  window=1;
  inew=0;
  list[n].position=pack(INCY/2, INCY/2);

  while (i0<=n) {
    newlist[inew].position= pos;
    newlist[inew++].value= window;

    window=(window<<3)&0x1ff;

    if (!window) {
        pos=list[i0].position-2-INCY;
        if (pos>list[i1].position-2) pos=list[i1].position-2;
        if (pos>list[i2].position-2+INCY) pos=list[i2].position-2+INCY;
      }

    if (list[i0].position<=pos+2+INCY) {
      window|=1; 
      i0++;
    }

    if (list[i1].position<=pos+2) {
      window|=2;
      i1++;
    }

    if (list[i2].position<=pos+2-INCY) {
      window|=4;
      i2++;
    }

    pos++;

  }

  return inew;
}




int combineListsMin(Cell *list1, int n1, Cell *list2, int n2, Cell *list3)  {
    int i,j,n3;

    i=0; j=0; n3=0;

    list1[n1].position= INT_MAX;
    list2[n2].position= INT_MAX;

    while (i<n1 || j<n2) {

       if (list1[i].position == list2[j].position) {
         if (list1[i].value < list2[j].value) {
             list3[n3]= list1[i];
         } else  {
             list3[n3]= list2[j];
         }

         i++;
         j++;
         n3++; 
       } else if (list1[i].position < list2[j].position) {
         list3[n3].position = list1[i].position;
         list3[n3++].value = list1[i++].value;
       } else {
         list3[n3].position = list2[j].position;
         list3[n3++].value = list2[j++].value;
       }
     }

    return n3;
  }



void extractCenterCells(Cell *list, int n, Cell *extractFrom) {

  int i,j;

  i=0; j=0;
/*
    printf("=======\n");
*/
  while (i<n) {
/*
    printf("!! %x %d %x %d %d !!\n", list[i].position, i,
                                  extractFrom[j].position, j,
                                  extractFrom[j].value);
*/
    if ( list[i].position == extractFrom[j].position ) {
      extractFrom[j].value--;
      i++;
      j++;
    } else {
      j++;
    }
  }

}

int getValues(Cell *cellList, int n, Cell *valueList) {

  int i,j;
  int noChange;

  i=0; j=0;

  noChange=1;

  while (i<n) {

    if ( cellList[i].position == valueList[j].position ) {
      if (cellList[i].value != valueList[j].value) {
        cellList[i].value= valueList[j].value;
        noChange= 0;
      }
      i++;
      j++;
    } else {
      j++;
    }

  }

  return noChange;
}

int pruneList(Cell *keepCells, int n, Cell *inList) {

  int i,j;

  i=0; j=0;

  while (i<n) {

    if ( keepCells[i].position == inList[j].position ) {
      inList[i]= inList[j];
      i++;
      j++;
    } else {
      j++;
    }

  }

  return n;
}

int uniqCells(Cell *list, int n) {

  int i, j, nnew;

  list[n].position= INT_MAX;

  i=0; j=0;
  while (i<n) {
     list[j]=list[i];

     while(list[i].position==list[j].position) i++;

     j++;
  }

  return j;
}

int copyList(Cell *fromList, int n, Cell *toList, int translateBy) {

  int i;

  for (i=0; i<n; i++) {
    toList[i].position= fromList[i].position+translateBy;
    toList[i].value= fromList[i].value;
  }

  return n;
}

void setValues(Cell *list, int n, int value) {
 int i;

  for (i=0; i<n; i++) {
    list[i].value= value;
  }

}

int countGreaterThan(Cell *list, int n, int value) {
  int i, j;

  j=0;
  for (i=0; i<n; i++) {

     if (list[i].value>value) {
        j++;
     }

  } 

  return j;
}

int removeGreaterThan(Cell *list, int n, int value) {
  int i, j;

  j=0;
  for (i=0; i<n; i++) {

     if (list[i].value<=value) {
        list[j++]=list[i];
     }

  } 

  return j;
}

int removeIfEquals(Cell *list, int n, int value) {
  int i, j;

  j=0;
  for (i=0; i<n; i++) {

     if (list[i].value!=value) {
        list[j++]=list[i];
     }

  } 

  return j;
}

int removeLessThan(Cell *list, int n, int value) {
  int i, j;

  j=0;
  for (i=0; i<n; i++) {

     if (list[i].value>=value) {
        list[j++]=list[i];
     }

  } 

  return j;
}

int removeIfZero(Cell *list, int n, int value) {
  int i, j;

  j=0;
  for (i=0; i<n; i++) {

     if (list[i].value) {
        list[j++]=list[i];
     }

  } 

  return j;
}

int condCopyList(Cell *fromList, int n, Cell *toList, int value) {

  int i,j;

  j=0;
  for (i=0; i<n; i++) {
    if (fromList[i].value == value) {
       toList[j].position= fromList[i].position;
       toList[j].value= fromList[i].value;
       j++;
    }
  }

  return j;
}

int getQuadrantCells(Cell *fromList, int n, Cell *toList, int origin,
                                           int dx, int dy) {

  int i,j;

  j=0;
  for (i=0; i<n; i++) {
    int x,y;

    unpack(fromList[i].position - origin, &x, &y);

    if (x*dx > 0 && y*dy > 0) {
       toList[j].position= fromList[i].position;
       toList[j].value= fromList[i].value;
       j++;
    }
  }

  return j;
}


int convolve(Cell *list1, int n1, Cell *list2, int n2, 
             Cell **convolution, Cell **scratch1, Cell **scratch2,
             void (*morespace) (int)) {

  /* morespace is a function that allocates space for convolution
     as needed */ 
    int i;
    int nconv=0; 

    for (i=n1-1; i>=0; i--) {

        morespace(n2);
        (void) copyList(list2, n2, *scratch1, -list1[i].position); 
        morespace(nconv+n2);
        nconv= combineLists(*scratch1, n2, *convolution, nconv, *scratch2); 
        (void) copyList(*scratch2, nconv, *convolution, 0);

    }

    return nconv;
}

int convolveMin(Cell *list1, int n1, Cell *list2, int n2, 
                Cell *convolution, Cell *scratch1, Cell *scratch2) {

    int i;
    int nconv=0; 
   
/* note that this operation is not commutative.
   min values from list2 are accumulated, while
   values in list1 are ignored */
 
    for (i=n1-1; i>=0; i--) {

        (void) copyList(list2, n2, scratch1, -list1[i].position); 
        nconv= combineListsMin(scratch1, n2, convolution, nconv, scratch2); 
        (void) copyList(scratch2, nconv, convolution, 0);

    }

    return nconv;
}

void quickSortCells(Cell list[], int left, int right) {

   /* direct QuickSort adapted from "C programming language,
      second edition."  We would have to allocate an
      array of pointers to use stdlib qsort.  This
      is probably simpler for now. */

    int i, last;
    Cell tmp;

    if (left >= right) return;

    tmp=list[left];
    list[left]=list[(left+right)/2];
    list[(left+right)/2]=tmp;

    last=left;

    for (i= left+1; i<=right; i++) {
        if (list[i].position < list[left].position) {
            last++; 

            tmp=list[i];
            list[i]=list[last];
            list[last]=tmp;
        }
    }

    tmp=list[left];
    list[left]=list[last];
    list[last]=tmp;

    quickSortCells(list, left, last-1);
    quickSortCells(list, last+1, right);

}



void sortRowMajor(Cell *list, int n) {

    quickSortCells(list, 0, n-1);

}

int pack(int x, int y) {

   return (y+CENTER)*INCY + x + CENTER;

}

int packtrans(int x, int y) {
  
   return pack(x,y)-CENTER*INCY-CENTER;

}

void unpack(int packed, int *x, int *y) {

    *y=packed/INCY - CENTER;
    *x=(packed & (INCY -1)) - CENTER;
    
}

void unpacktrans(int packed, int *tx, int *ty) {

    unpack(packed+CENTER*INCY + CENTER, tx, ty);

}


void transpose(Cell *list, int n) {
  
  int i;

   for (i=0; i<n; i++) {
     int x,y;

     unpack(list[i].position, &x, &y);
     list[i].position=pack(y, x);

   }

   sortRowMajor(list, n);
}

void flipx(Cell *list, int n) {
  
  int i;

   for (i=0; i<n; i++) {
     int x,y;

     unpack(list[i].position, &x, &y);
     list[i].position=pack(-x, y);

   }

   sortRowMajor(list, n);
}

void flipy(Cell *list, int n) {
  
  int i;

   for (i=0; i<n; i++) {
     int x,y;

     unpack(list[i].position, &x, &y);
     list[i].position=pack(x, -y);

   }

   sortRowMajor(list, n);
}

void dumpCellBits(Cell *list, int n) {

  int i; 

  for (i=0; i<n; i++) {

    int x,y;
    int j;

    unpack(list[i].position, &x, &y);

    printf("%5d %5d ", x, y); 

    for (j=31; j>=0; j--) { 
        printf("%d", (list[i].value & 1<<j) >>j);
    }
    printf("\n");

  }

}

void dumpCellHex(Cell *list, int n) {

  int i; 

  for (i=0; i<n; i++) {

    int x,y;
    int j;

    unpack(list[i].position, &x, &y);

    printf("%5d %5d %08x\n", x, y, list[i].value); 

  }

}

void dumpCells(Cell *list, int n) {

  int i; 

  for (i=0; i<n; i++) {

    int x,y;

    unpack(list[i].position, &x, &y);

    printf("%5d %5d %5d\n", x, y, list[i].value);

  }

}

void dumpTransCells(Cell *list, int n) {

  int i; 

  for (i=0; i<n; i++) {

    int x,y;

    unpacktrans(list[i].position, &x, &y);

    printf("%5d %5d %5d\n", x, y, list[i].value);

  }

}

int firstZero(Cell *list, int n) {
    
 int i;
 int transl;

 assert(n>=0);

 if (n==0) transl = 0;
 else { 
   transl=list[0].position - pack(0,0);  

   for (i=0; i<n; i++) {
     list[i].position -= transl;
   }
 }

 return transl; 

}


int compare(Cell *list1, Cell *list2, int n) {

  int i;

  for (i=0; i<n; i++) {
     if (list1[i].position < list2[i].position) {
        return -1;
     } else if (list1[i].position > list2[i].position) {
        return 1;
     }
  }

  return 0;

}


BoundingBox makeBoundingBox(Cell *list, int n)  {

  BoundingBox bb;
  int i;

  bb.lx=bb.ly= INT_MAX;
  bb.ux=bb.uy= INT_MIN;

  for (i=0; i<n; i++) {
    int x,y;

    unpack(list[i].position, &x, &y);

    if (x<bb.lx) bb.lx=x;
    if (x>bb.ux) bb.ux=x;

    if (y<bb.ly) bb.ly=y;
    if (y>bb.uy) bb.uy=y;

  }

  return bb;

}

void makeString(Cell *list, int n, char *patstring) {

  BoundingBox bb;
  int px, py;
  int i;

  bb=makeBoundingBox(list, n);

  px=bb.lx;
  py=bb.ly;

  for (i=0; i<n; i++) {
     int x,y;

     unpack(list[i].position, &x, &y);

     while (py<y) {
        *(patstring++)='!';
        px=bb.lx;
        py++;
     }

     while (py==y && px<x) {
        *(patstring++)='.';
        px++;
     }

     if (list[i].value>64 && list[i].value<128) {
         *(patstring++)= (char) list[i].value;
     } else{
         *(patstring++)='*';
     }
     px++;
   }

   *(patstring++)='!';

   *patstring='\0';

}

void transformBack(Cell *list, Transformation T, int n) {

    copyList(list, n, list, T.translateBy);
    if (T.transposeF) transpose(list, n);
    if (T.flipyF) flipy(list, n);
    if (T.flipxF) flipx(list, n);

}

void transform(Cell *list, Transformation T, int n) {

    if (T.flipxF) flipx(list, n);
    if (T.flipyF) flipy(list, n);
    if (T.transposeF) transpose(list, n);
    copyList(list, n, list, -T.translateBy);

}


