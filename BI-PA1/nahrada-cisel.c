/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#endif

void pridat (char **retezec, int *retezecDelka, int *retezecMax,const char *pridatRetezec, int pridatDelka) { /*po skonceni fce je v retezec ukazatel na konec retezce*/
	if (*retezecDelka+pridatDelka >= *retezecMax) {
		*retezecMax *= 2;
		if (*retezecMax == 0) *retezecMax = 512;
		*retezec = (char*) realloc (*retezec, *retezecMax);
		if (*retezec == NULL) { /*otestuji zda se realloc zdaril*/
			exit(1); /*pokud ne ukoncim program*/
		}
		memset (*retezec+*retezecDelka, 0, *retezecMax-*retezecDelka); /*nove alokovane pole vyplnim nulami*/
	}
	memcpy (*retezec+*retezecDelka, pridatRetezec, pridatDelka); /*prikopiruju obsah*/
	*retezecDelka += pridatDelka; /*zvysim delku*/
}

void nahrad (int n, char **retezec, int *retezecDelka, int *retezecMax) { /*funkce nahrazuje cislo textovou reprezentaci*/
	int cif;
	if (n < 0) pridat (retezec, retezecDelka, retezecMax, "minus ", 6); /*osetrim pripad zaporneho vstupu*/

	cif = n / 1000000000; /*pocet miliard*/
	if (cif < 0) cif *= -1; /*zde muze byt cislo stale zaporne, nemohu vynasobit rovnou cele cislo, -MaxInt by pote pretekl*/
	switch (cif) {
		case 1: pridat (retezec, retezecDelka, retezecMax, "jedna miliarda ", 15); break;
		case 2: pridat (retezec, retezecDelka, retezecMax, "dve miliardy ", 13); break;
		/*vyssi hodnoty nemusim resit, MaxInt je cca 2 mld*/
	}
	n %= 1000000000;
	if (n < 0) n*=-1; /*zaporne cislo si prevedu na kladne*/

	/*------------------------------------MILIONY-------------------------------------------*/

	cif = n / 1000000; /*pocet milionu*/
	if (cif > 0) {
		if (cif % 100 > 10 && cif % 100 < 20) { /*osetrim pripad 11-19 milionu*/
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "milionu ", 8);
		}
		else if (cif % 10 == 1) {
			nahrad(cif-1, retezec, retezecDelka, retezecMax);
			pridat(retezec, retezecDelka, retezecMax, "jeden milion ", 13);
		}
		else if (cif % 10 > 1 && cif % 10 < 5) {
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "miliony ", 8);
		}
		else {
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "milionu ", 8);
		}
	}
	n %= 1000000;

	/*------------------------------------TISICE-------------------------------------------*/

	cif = n / 1000;
	if (cif > 0) {
		if (cif % 100 > 10 && cif % 100 < 20) { /*osetrim pripad 11-19 tisic*/
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "tisic ", 6);
		}
		else if (cif % 10 == 1) {
			nahrad(cif-1, retezec, retezecDelka, retezecMax);
			pridat(retezec, retezecDelka, retezecMax, "jeden tisic ", 12);
		}
		else if (cif % 10 > 1 && cif % 10 < 5) {
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "tisice ", 7);
		}
		else {
			nahrad (cif, retezec, retezecDelka, retezecMax);
			pridat (retezec, retezecDelka, retezecMax, "tisic ", 6);
		}
	}
	n %= 1000;

	/*------------------------------------STOVKY-------------------------------------------*/

	cif = n / 100;
	if (cif > 0) {
		switch (cif) {
			case 1: pridat(retezec, retezecDelka, retezecMax, "jedno sto ", 10); break;
			case 2: pridat(retezec, retezecDelka, retezecMax, "dve ste ", 8); break;
			case 3: pridat(retezec, retezecDelka, retezecMax, "tri sta ", 8); break;
			case 4: pridat(retezec, retezecDelka, retezecMax, "ctyri sta ", 10); break;
			case 5: pridat(retezec, retezecDelka, retezecMax, "pet set ", 8); break;
			case 6: pridat(retezec, retezecDelka, retezecMax, "sest set ", 9); break;
			case 7: pridat(retezec, retezecDelka, retezecMax, "sedm set ", 9); break;
			case 8: pridat(retezec, retezecDelka, retezecMax, "osm set ", 8); break;
			case 9: pridat(retezec, retezecDelka, retezecMax, "devet set ", 10); break;
		}
	}
	n %= 100;

	/*------------------------------------DESITKY-------------------------------------------*/

	cif = n / 10;
	if (cif > 0) {
		switch (cif) {
			case 1: /*vyresim pripad 11-19*/
				switch (n) {
					case 10: pridat(retezec, retezecDelka, retezecMax, "deset ", 6); return;
					case 11: pridat(retezec, retezecDelka, retezecMax, "jedenact ", 9); return;
					case 12: pridat(retezec, retezecDelka, retezecMax, "dvanact ", 8); return;
					case 13: pridat(retezec, retezecDelka, retezecMax, "trinact ", 8); return;
					case 14: pridat(retezec, retezecDelka, retezecMax, "ctrnact ", 8); return;
					case 15: pridat(retezec, retezecDelka, retezecMax, "patnact ", 8); return;
					case 16: pridat(retezec, retezecDelka, retezecMax, "sestnact ", 9); return;
					case 17: pridat(retezec, retezecDelka, retezecMax, "sedmnact ", 9); return;
					case 18: pridat(retezec, retezecDelka, retezecMax, "osmnact ", 8); return;
					case 19: pridat(retezec, retezecDelka, retezecMax, "devatenact ", 11); return;
				}
				break;
			case 2: pridat(retezec, retezecDelka, retezecMax, "dvacet ", 7); break;
			case 3: pridat(retezec, retezecDelka, retezecMax, "tricet ", 7); break;
			case 4: pridat(retezec, retezecDelka, retezecMax, "ctyricet ", 9); break;
			case 5: pridat(retezec, retezecDelka, retezecMax, "padesat ", 8); break;
			case 6: pridat(retezec, retezecDelka, retezecMax, "sedesat ", 8); break;
			case 7: pridat(retezec, retezecDelka, retezecMax, "sedmdesat ", 10); break;
			case 8: pridat(retezec, retezecDelka, retezecMax, "osmdesat ", 9); break;
			case 9: pridat(retezec, retezecDelka, retezecMax, "devadesat ", 9); break;
		}
	}
	n %= 10;

	/*------------------------------------JEDNOTKY-------------------------------------------*/

	cif = n;
	if (cif > 0) {
		switch (cif) {
			case 1: pridat(retezec, retezecDelka, retezecMax, "jedna ", 6); break;
			case 2: pridat(retezec, retezecDelka, retezecMax, "dva ", 4); break;
			case 3: pridat(retezec, retezecDelka, retezecMax, "tri ", 4); break;
			case 4: pridat(retezec, retezecDelka, retezecMax, "ctyri ", 6); break;
			case 5: pridat(retezec, retezecDelka, retezecMax, "pet ", 4); break;
			case 6: pridat(retezec, retezecDelka, retezecMax, "sest ", 5); break;
			case 7: pridat(retezec, retezecDelka, retezecMax, "sedm ", 5); break;
			case 8: pridat(retezec, retezecDelka, retezecMax, "osm ", 4); break;
			case 9: pridat(retezec, retezecDelka, retezecMax, "devet ", 6); break;
		}
	}
}

char podminkaPred(char predchozi) {
	if (predchozi == ' ' || predchozi == '\n') return 1;
	return 0;
}

char podminkaZa(char nasledujici) {
	if (nasledujici == ' ' || nasledujici == '\n' || nasledujici == 0) return 1;
	return 0;
}

int strNaInt (char * start) {
  int vrat = 0;
	while (isdigit(*start)) vrat = vrat * 10 + *start-'0';
	return vrat;
}

char * replaceNumbers ( char * str ) {
	char * startInt, *endInt;
	char *ted = str;
	char * out = NULL;
	char pred = ' ';
	int outMax = 0, outLen = 0;
	int cislo;

	while (*ted != 0) { /*projdu cely retezec*/
		if ((isdigit(*ted) || *ted=='+' || *ted=='-') && podminkaPred(pred)) { /*zkontroluji zda jsem nacetl cislo vyhovujici podminkam*/
			startInt = endInt = ted; /*pokud ano, zapamatuji si pozici*/
			while(isdigit(*(++endInt))); /*ulozim si pozici kde cislo konci*/
			if (podminkaZa(*endInt)) { /*pokud vyhovi podminkam*/
				cislo = strNaInt(startInt, endInt); /*prevedu si retezec na cislo*/
				if (cislo == 0) pridat(&out, &outLen, &outMax, "nula ", 5); /*pokud jsem dostal 0 vypisu ji a mohu skoncit*/
				else nahrad(cislo, &out, &outLen, &outMax); /*zavolam funkci na nahrazeni*/
				*(out+outLen-1)=0; /*nahrazeny retezec ukoncim 0*/
				outLen--; /*snizim delku o tu jedno 0*/
			}
			else pridat (&out, &outLen, &outMax, startInt, endInt-startInt); /*pokud za cislem nenasledoval bily znak, tak retezec zkopiruji*/
			pred = *(endInt-2); /*nastavim ukazatel na predchozi na konec -2 (budu jeste inkrementovat)*/
			ted = endInt-1;
		}
		else pridat(&out, &outLen, &outMax, ted, 1); /*pokud jsem nenacetl cislici tak znak zkopiruji na vystup*/

		pred = *ted; /*posunu se na dalsi znak*/
		ted++;
	}

	return out;
}

int main(void) {
	char * r;
	char * a = (char*)malloc(sizeof(char*));
  r = replaceNumbers (fgets (a,1024*1024,stdin));

  printf("%s\n", r);

  free(a);
  free(r);
	return 0;
}
