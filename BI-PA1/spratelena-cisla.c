/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
	unsigned int n = 2000000, i;
	unsigned int * pole;
	unsigned int last;

	printf("Zadejte limit:\n");
	if (scanf("%u", &n) != 1 || n <= 0) { /*zkontroluji platnost vstupu*/
		printf("Nespravny vstup.\n");

		return 1;
	}

	/*hledani spratelenych cisel*/
	pole = (unsigned int*)malloc((n+1)*sizeof (*pole)); /*naalokuji si dynamicky pole, abych neplytval pameti*/
	
	for (i = 2; i <= n; i++) pole[i] = 1; /*vsem prvkum pole nastavim hodnotu na 1, 1 deli vsechna cisla*/

	for (i = 2; i <= n/2; i++) { /*vemu kazde cislo od 1 do n/2 a prictu ho do souctu vsech jeho nasobku*/
		last = i; /*protoze se do souctu delitelu nepocita cislo samotne zacinam az od druheho nasobku*/
		while (last+i <= n) { /*pouziji scitani, jednodussi => rychlejsi operace*/
			pole[last+i] += i;
			last += i;
		}
	}

	/*for (i = 2; i <= n; i++)*/	/*vypis paru*/
    /*zkontroluji, zda hodnota pole[i] je stale v poli, abych se nedostal mimo*/
		/*if (pole[i] <= n && i == pole[pole[i]] && i < pole[i])*/ /*zjistim, zda pole[i] vypsat a zajistim aby se par nevypsal 2x*/
    /*  printf("%d, %d\n", i, pole[i]); */

	free(pole); /*pole jiz nepotrebuji tak uvolnim pamet*/

  return 0;
}
