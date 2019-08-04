#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Fill an array without going through it */

#define ASIZE 5000
int main (int argc, char ** argv) {
  int array[ASIZE];
  int b = 0;
  int e = ASIZE - 1;
  int pe = e;
  unsigned int m = 0;
  int i = 0;
  int count = 0;
  clock_t start;
  clock_t stop;
  
  memset(array, 0, ASIZE * sizeof(array[0]));
  start = clock();
  for (i = 0; i < ASIZE; i++) {
    b = 0;
    e = ASIZE - 1;
    pe = e;
    while (1) {
      count++;
      if (e < b) {
	if (array[b] == 0) {
	  array[b]++;
	}
	break;
      }
      m = (e + b) >> 1;
      if (array[m] == 0) {
	pe = e;
  	e = m - 1;
      } else {
	b = m + 1;
	e = pe - 1;
      }
    }
  }
  stop = clock();  
  fprintf(stderr, "Number of loop : %d (%e)\n", count, ((double)(stop - start) / CLOCKS_PER_SEC));
  
  count = 0;
  memset(array, 0, ASIZE * sizeof(array[0]));
  start = clock();
  for (i = 0; i < ASIZE; i++) {
    b = 0;
    while (array[b] == 0) { b++; count++;}
    if (b < ASIZE) {
      array[b]++;
    }
  }
  stop = clock();
  fprintf(stderr, "Number of loop : %d (%e)\n", count, ((double)(stop - start) / CLOCKS_PER_SEC));
  
  return 0;
}
