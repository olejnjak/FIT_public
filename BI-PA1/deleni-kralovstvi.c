/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#endif

/* pripadne Vase pomocne funkce */

double sqr (double cislo) {
  return cislo*cislo;
}

double obsahTroj (double x[], double y[]) {
  /*spocitam si jednotlive strany trojuhelnika*/
  double sA = fabs(sqrt(sqr(x[0]-x[1])+sqr(y[0]-y[1])));
  double sB = fabs(sqrt(sqr(x[1]-x[2])+sqr(y[1]-y[2])));
  double sC = fabs(sqrt(sqr(x[0]-x[2])+sqr(y[0]-y[2])));

  /*pomoci Heronova vzorce spocitam obsah*/
  double s = (sA + sB + sC)/2;
  return sqrt(s*(s-sA)*(s-sB)*(s-sC));
}

int divideLand ( double x[], double y[], int nr, double * diff ){
  double * obsahy;
  int pevnyBod, i;
  char pocitejObsah = 1;
  double tmpX[3], tmpY[3], obsahTmp, rozdil = -1;
  double obsahA, obsahB;

  if (nr < 4) return 0; /*osetrim pripad kdy nelze pozemky rozdelit*/

  obsahy = (double*)malloc((nr-1)*sizeof (*obsahy));
  obsahy[0] = 0; /*nulty prvek vyuziji k ulozeni celkoveho obsahu*/

  for (pevnyBod = 0; pevnyBod < nr; pevnyBod++) { /*v cyklu vyberu vsechny body jako stred vejire*/
    for (i = 1; i <= nr-2; i++) {  /*zajistim spocitani obsahu vsech trojuhelniku ve vejiri*/
      tmpX[0] = x[pevnyBod];
      tmpX[1] = x[(pevnyBod + i) % nr];
      tmpX[2] = x[(pevnyBod + i + 1) % nr];
      tmpY[0] = y[pevnyBod];
      tmpY[1] = y[(pevnyBod + i) % nr];
      tmpY[2] = y[(pevnyBod + i + 1) % nr];
      obsahy[i] = obsahTroj(tmpX, tmpY); /*obsah si ulozim do pole*/
      if (pocitejObsah == 1) obsahy[0] += obsahy[i];
    }
    pocitejObsah = 0;
    obsahTmp = 0;
    /*z pole z obsahu budu postupne vybirat hodnoty a zkontroluji zda rozdil mezi dosud vybranymi trojuhelniky a zbyvajici plochou
    je mensi nez predchozi nejlepsi rozdil*/
    for (i = 1; i <= nr-2; i++) printf("troj %d -> %f\n", i, obsahy[i]);
    for (i = 1; i <= nr-3; i++) {
      obsahTmp += obsahy[i];
      obsahA = obsahTmp;
      obsahB = obsahy[0] - obsahA;
      if (rozdil == -1 || rozdil > fabs(obsahB - obsahA)) { /*rozdilu jsem nastavil pocatecni hodnotu na -1 abych jej odlisil pri prvnim pruchodu*/
        rozdil = fabs(obsahB - obsahA);
      }
      if (rozdil == 0) break; /*pokud narazim na rozdil = 0 lepsi uz najit nelze tak funkci ukoncim*/
    }
    if (rozdil == 0) break;
  }
	printf("celkem %f", obsahy[0]);
  *diff = rozdil;
  free(obsahy);
  return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] ){
  double diff;
	double x0[4] = { 0, 50, 60, 0 };
	double y0[4] = { 0, 0, 60, 50 };
  divideLand(x0,y0,4,&diff);
  return 1;
}
#endif

