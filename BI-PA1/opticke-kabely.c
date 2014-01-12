/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>

/* compare funkce pro qsort */
int srovnej (const void * a, const void * b) {
  if (*(int*)a > *(int*)b) return 1;
  else if (*(int*)a < *(int*)b) return -1;
  return 0;
}

int main ( int argc, char * argv [] ) {
  int velikostPole = 10000, pocetPrvku = 0, vzdalenost = 0, i, stred;
  int * x = (int*) malloc(velikostPole*sizeof(*x));
  int * y = (int*) malloc(velikostPole*sizeof(*y));

  printf("Zadejte souradnice domu:\n");

  while (1) {
    if (pocetPrvku == velikostPole) { /*realokace pole*/
      x = (int*) realloc (x, 2*velikostPole*sizeof(int));
      y = (int*) realloc (y, 2*velikostPole*sizeof(int));
      velikostPole *= 2;
    }
    if ( scanf ("%d %d", &x[pocetPrvku], &y[pocetPrvku]) != 2 && !feof(stdin)) { /*zkontroluji platnost vstupu*/
      free (x); free (y); /*uvolnim dynamickou pamet*/
      printf ("Nespravny vstup.\n");
      #ifndef __PROGTEST__
        system ( "pause" );
      #endif
      return 1;
    }
    if (feof (stdin)) break; /*pokud jsem na konci souboru ukoncim cyklus*/
    pocetPrvku++; /*pokud mam jeste cist, zvysim pocet prvku pole*/
  }

  qsort (x, pocetPrvku, sizeof(int), srovnej); /*seradim si souradnice at mohu urcit median*/
  qsort (y, pocetPrvku, sizeof(int), srovnej);
  stred = pocetPrvku / 2; /*median*/

  for (i = 0; i < pocetPrvku; i++) vzdalenost += abs(x[stred] - x[i]) + abs(y[stred] - y[i]); /*spocitam vzdalenosti*/

  printf("Delka rozvodu: %d\n", vzdalenost);
  free(x);
  free(y);

#ifndef __PROGTEST__
  system ( "pause" );
#endif
  return 0;
}
