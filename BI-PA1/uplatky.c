/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>
int q = 0;
int pocitejUplatky (int budget, int * table, int tableLen, int *meziVypocty) {
  int i, maxBribe = 0;
  q++;

  if (budget == 1) return table[1]; /*pokud mam rozpocet 1, lepsiho vysledku nez table[1] nelze dosahnout*/
  if (meziVypocty[budget] >= 0) return meziVypocty[budget]; /*zkontroluji zda uz vysledek pro tento rozpocet nemam spocitany*/
  for (i = 1; i < tableLen; i++) { /*projdu tabulku s uplatky*/
    if (budget-i<0) break; /*pokud bych se dostal do zaporneho rozpoctu, ukoncim cyklus*/
    int subBribe = table[i] + pocitejUplatky(budget-i, table, tableLen, meziVypocty); /*pro kazdout zakazku spocitam nejvetsi uplatek*/
    if (subBribe > maxBribe) maxBribe = subBribe; /*pokud je aktualni uplatek vetsi nez prozatimni maximum opravim maximum*/
  }
  meziVypocty[budget] = maxBribe; /*ulozim si spocitane maximum*/
  return maxBribe;
}

int sumBribes ( int budget, int * table, int tableLen ) {
  int *mezivypocty = (int*)malloc((budget+1)*sizeof(*mezivypocty)); /*alokuji si pole pro mezivysledky*/
  int i, vrat;
  q=0;
  for (i=0;i<=budget;i++) mezivypocty[i] = -1;  /*inicializuji pole pro mezivypocty*/
  vrat = pocitejUplatky (budget, table, tableLen, mezivypocty); /*vypocitam maximalni uplatek*/
  printf("%d\n", q);
  free(mezivypocty); /*uvolnim pole*/
  return vrat;
}

int main(){

int r;
int test1 [] = { 0, 100, 0, 0, 350, 0, 750 };
int test2 [] = { 0, 1, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 53 };
int test3 [] = { 0, 0, 0, 0, 0, 0, 0, 1 };
printf("test1 \n");
r = sumBribes ( 1, test1, 7 ); /* r = 100 */ printf("\t sum %d \n", r);
r = sumBribes ( 2, test1, 7 ); /* r = 200 */ printf("\t sum %d \n", r);
r = sumBribes ( 3, test1, 7 ); /* r = 300 */ printf("\t sum %d \n", r);
r = sumBribes ( 4, test1, 7 ); /* r = 400 */ printf("\t sum %d \n", r);
r = sumBribes ( 5, test1, 7 ); /* r = 500 */ printf("\t sum %d \n", r);
r = sumBribes ( 6, test1, 7 ); /* r = 750 */ printf("\t sum %d \n", r);
r = sumBribes ( 7, test1, 7 ); /* r = 850 */ printf("\t sum %d \n", r);
r = sumBribes ( 8, test1, 7 ); /* r = 950 */ printf("\t sum %d \n", r);
r = sumBribes ( 9, test1, 7 ); /* r = 1050 */ printf("\t sum %d \n", r);
r = sumBribes ( 10, test1, 7 ); /* r = 1150 */ printf("\t sum %d \n", r);
r = sumBribes ( 11, test1, 7 ); /* r = 1250 */ printf("\t sum %d \n", r);
r = sumBribes ( 12, test1, 7 ); /* r = 1500 */ printf("\t sum %d \n", r);
r = sumBribes ( 13, test1, 7 ); /* r = 1600 */ printf("\t sum %d \n", r);
r = sumBribes ( 14, test1, 7 ); /* r = 1700 */ printf("\t sum %d \n", r);
r = sumBribes ( 15, test1, 7 ); /* r = 1800 */ printf("\t sum %d \n", r);
r = sumBribes ( 16, test1, 7 ); /* r = 1900 */ printf("\t sum %d \n", r);
r = sumBribes ( 17, test1, 7 ); /* r = 2000 */ printf("\t sum %d \n", r);
printf("test2 \n");
r = sumBribes ( 1, test2, 16 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 2, test2, 16 ); /* r = 7 */ printf("\t sum %d \n", r);
r = sumBribes ( 3, test2, 16 ); /* r = 8 */ printf("\t sum %d \n", r);
r = sumBribes ( 4, test2, 16 ); /* r = 14 */ printf("\t sum %d \n", r);
r = sumBribes ( 5, test2, 16 ); /* r = 15 */ printf("\t sum %d \n", r);
r = sumBribes ( 6, test2, 16 ); /* r = 21 */ printf("\t sum %d \n", r);
r = sumBribes ( 7, test2, 16 ); /* r = 22 */ printf("\t sum %d \n", r);
r = sumBribes ( 8, test2, 16 ); /* r = 28 */ printf("\t sum %d \n", r);
r = sumBribes ( 9, test2, 16 ); /* r = 29 */ printf("\t sum %d \n", r);
r = sumBribes ( 10, test2, 16 ); /* r = 35 */ printf("\t sum %d \n", r);
r = sumBribes ( 11, test2, 16 ); /* r = 36 */ printf("\t sum %d \n", r);
r = sumBribes ( 12, test2, 16 ); /* r = 42 */ printf("\t sum %d \n", r);
r = sumBribes ( 13, test2, 16 ); /* r = 43 */ printf("\t sum %d \n", r);
r = sumBribes ( 14, test2, 16 ); /* r = 49 */ printf("\t sum %d \n", r);
r = sumBribes ( 15, test2, 16 ); /* r = 53 */ printf("\t sum %d \n", r);
r = sumBribes ( 16, test2, 16 ); /* r = 56 */ printf("\t sum %d \n", r);
r = sumBribes ( 17, test2, 16 ); /* r = 60 */ printf("\t sum %d \n", r);
printf("test3 \n");
r = sumBribes ( 1, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 2, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 3, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 4, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 5, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 6, test3, 8 ); /* r = 0 */ printf("\t sum %d \n", r);
r = sumBribes ( 7, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 8, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 9, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 10, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 11, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 12, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 13, test3, 8 ); /* r = 1 */ printf("\t sum %d \n", r);
r = sumBribes ( 14, test3, 8 ); /* r = 2 */ printf("\t sum %d \n", r);
r = sumBribes ( 15, test3, 8 ); /* r = 2 */ printf("\t sum %d \n", r);
r = sumBribes ( 16, test3, 8 ); /* r = 2 */ printf("\t sum %d \n", r);
r = sumBribes ( 17, test3, 8 ); /* r = 2 */ printf("\t sum %d \n", r);

system("pause");
return 0;
}
