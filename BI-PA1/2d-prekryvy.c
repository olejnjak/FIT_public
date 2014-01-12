/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void chybaVstup() {
	printf("Nespravny vstup.\n");
#ifndef __PROGTEST__
  system ( "pause" );
#endif
  exit (1);
}

void vystup (int kod) {
	if (kod == 0) printf("Prunik je prazdny.\n");
	else printf ("Prunik je neprazdny.\n");
#ifndef __PROGTEST__
  system ( "pause" );
#endif
  exit (0);
}

void nactiRect(int* ax, int* ay, int* bx, int* by) {
	int pom;

	printf("Prvni bod:\n");
	if (scanf("%d %d", ax, ay) != 2) chybaVstup();
	printf("Druhy bod:\n");
	if (scanf("%d %d", bx, by) != 2) chybaVstup();

	if (*ax == *bx || *ay == *by) chybaVstup();
	/*pokud jsem nedostal body A a C, tak vymenim souradnice*/
	if (*ax > *bx) {
		pom = *ax;
		*ax = *bx;
		*bx = pom;
	}
	if (*ay > *by) {
		pom = *ay;
		*ay = *by;
		*by = pom;
	}
}

void nactiCirc (int* sx, int* sy, int* r) {
	printf("Stred:\n");
	if (scanf("%d %d", sx, sy) != 2) chybaVstup();
	printf("Polomer:\n");
	if (scanf("%d", r) != 1) chybaVstup();

	if (*r <= 0) chybaVstup();
}

double vzdalenostBodu (int ax, int ay, int bx, int by) {
	return sqrt(abs(ax-bx)*abs(ax-bx)+abs(ay-by)*abs(ay-by));
}

int prunikInt(int start1, int end1, int start2, int end2) { /*spocita zda existuje prunik dvou intervalu*/
	int pom;
	if (start1 > start2) {
		pom = start1;
		start1 = start2;
		start2 = pom;
		pom = end1;
		end1 = end2;
		end2 = pom;
	}
	if (start1 <= start2 && end1 >= start2) return 1;
	else if (start2 <= start1 && end2 >= start1) return 1;
	return 0;
}

void prunikRR (int rect1_ax, int rect1_ay, int rect1_bx, int rect1_by, int rect2_ax, int rect2_ay, int rect2_bx, int rect2_by) {
	if (prunikInt(rect1_ax, rect1_bx, rect2_ax, rect2_bx) && prunikInt(rect1_ay, rect1_by, rect2_ay, rect2_by)) vystup(1);
	else vystup (0);
}

void prunikCC (int circ1_sx, int circ1_sy, int circ1_r, int circ2_sx, int circ2_sy, int circ2_r) {
	if (vzdalenostBodu(circ1_sx, circ1_sy, circ2_sx, circ2_sy) <= circ1_r + circ2_r) vystup(1);
	/*if (fabs(vzd - circ1_r - circ2_r) <= circ1_r*1e-12) vystup(1); musim pocitat s nepresnosti double*/
	else vystup (0);
}

void prunikRC (int ax, int ay, int bx, int by, int sx, int sy, int r) {
	if (ax <= sx && bx >= sx && ay <= sy && by >= sy) vystup(1); /*otestuji zda stred k neni v obdelniku*/
	else if (fabs(vzdalenostBodu(sx, sy, ax, ay) - r) <= r*1e-12) vystup(1); /*otestuji zda nektery z vrcholu obdelnika nelezi v kruhu*/
	else if (fabs(vzdalenostBodu(sx, sy, bx, ay) - r) <= r*1e-12) vystup(1);
	else if (fabs(vzdalenostBodu(sx, sy, bx, by) - r) <= r*1e-12) vystup(1);
	else if (fabs(vzdalenostBodu(sx, sy, ax, by) - r) <= r*1e-12) vystup(1); /*zbyva osetrit pripad kdy obdelnik sekne kruh*/
	else if (sy + r >= ay && sy <= ay && sx >= ax && sx <= bx) vystup(1); /*strana a sekne kruh*/
	else if (sy - r <= by && sy >= by && sx >= ax && sx <= bx) vystup(1); /*strana c sekne kruh*/
	else if (sx + r >= ax && sx <= ax && sy >= ay && sy <= by) vystup(1); /*strana d sekne kruh*/
	else if (sx - r <= bx && sx >= bx && sy >= ay && sy <= by) vystup(1); /*strana b sekne kruh*/
	else vystup (0);
}

int main ( int argc, char * argv [] )
{
	char tvar1, tvar2;
	int rect1_ax, rect1_ay, rect1_bx, rect1_by;
	int rect2_ax, rect2_ay, rect2_bx, rect2_by;
	int circ1_sx, circ1_sy, circ1_r;
	int circ2_sx, circ2_sy, circ2_r;

	printf("Tvar 1 (R=rectangle, C=circle):\n");
	tvar1 = getc(stdin);
	if (getc(stdin) != '\n') chybaVstup();
	if (tvar1 == 'R') nactiRect(&rect1_ax, &rect1_ay, &rect1_bx, &rect1_by);
	else if (tvar1 == 'C') nactiCirc(&circ1_sx, &circ1_sy, &circ1_r);
	else chybaVstup();

	while(getc(stdin) != '\n');
	printf("Tvar 2 (R=rectangle, C=circle):\n");

	tvar2 = getc(stdin);
	if (getc(stdin) != '\n') chybaVstup();
	if (tvar2 == 'R') nactiRect(&rect2_ax, &rect2_ay, &rect2_bx, &rect2_by);
	else if (tvar2 == 'C') nactiCirc(&circ2_sx, &circ2_sy, &circ2_r);
	else chybaVstup();

	if (tvar1 == 'R' && tvar2 == 'R') prunikRR(rect1_ax, rect1_ay, rect1_bx, rect1_by, rect2_ax, rect2_ay, rect2_bx, rect2_by);
	else if (tvar1 == 'C' && tvar2 == 'C') prunikCC(circ1_sx, circ1_sy, circ1_r, circ2_sx, circ2_sy, circ2_r);
	else {
		if (tvar2 == 'R') {
			rect1_ax = rect2_ax;
			rect1_ay = rect2_ay;
			rect1_bx = rect2_bx;
			rect1_by = rect2_by;
		}
		else if (tvar2 == 'C') {
			circ1_sx = circ2_sx;
			circ1_sy = circ2_sy;
			circ1_r = circ2_r;
		}
		prunikRC(rect1_ax, rect1_ay, rect1_bx, rect1_by, circ1_sx, circ1_sy, circ1_r);
	}

#ifndef __PROGTEST__
  system ( "pause" );
#endif
  return 0;
}
