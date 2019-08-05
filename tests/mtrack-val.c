#include "mtrack.h"
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char ** argv) {
  int i = 0;
  int j = 0;
  MTracker t;
  MTracker * t2;
  
  for (i = 1; i < 1746; i++) {
    mk_init(&t);
    for (j = 0; j < i; j++) {
      mk_track(&t, malloc(1000), free);
    }
    mk_destroy(&t);
  }

  for (i = 1; i < 2323; i++) {
    t2 = mk_alloc();
    for (j = 0; j < i; j++) {
      mk_track(t2, malloc(1000), free);
    }
    mk_destroy(t2);
  }
  
  return 0;
}
