/*
 *
 * Author: Jakub Olejnik
 * olejnjak@fit.cvut.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main ( int argc, char * argv [] )
{
	short scan;
	double a, b, c;
	double alfa, beta, gama;
	double alfas, betas, gamas;
	double alfam, betam, gamam;
	double alfasec, betasec, gamasec;

	printf("Zadejte velikost stran a b c:\n");
	scan = scanf ("%lf %lf %lf", &a, &b, &c);

	if (scan != 3 || (a <= 0 || b <=0 || c<=0)) {
		printf ("Nespravny vstup.\n");
		#ifndef __PROGTEST__
  		system ( "pause" );
		#endif /* __PROGTEST__ */
  	return 1;
	}

	if (a+b<=c || a+c<=b || c+b<=a) {
		printf ("Trojuhelnik neexistuje.\n");
		#ifndef __PROGTEST__
  		system ( "pause" );
		#endif /* __PROGTEST__ */
  	return 0;
	}
	else if (a == b && a == c) printf("Trojuhelnik je rovnostranny.\n");
	else if (a == b || a == c || b == c) printf("Trojuhelnik je rovnoramenny.\n");
	else printf("Trojuhelnik neni ani rovnostranny ani rovnoramenny.\n");

	alfa = acos((a*a-b*b-c*c)/(-2*b*c));
	beta = acos((b*b-c*c-a*a)/(-2*c*a));
	gama = acos((c*c-b*b-a*a)/(-2*b*a));

	alfas = alfa*180/M_PI;
	betas = beta*180/M_PI;
	gamas = gama*180/M_PI;

	alfam = (alfas - (int)alfas) * 60;
	betam = (betas - (int)betas) * 60;
	gamam = (gamas - (int)gamas) * 60;

	alfasec = (alfam - (int)alfam) * 60;
	betasec = (betam - (int)betam) * 60;
	gamasec = (gamam - (int)gamam) * 60;

	if ((a*a+b*b==c*c) || (a*a+c*c==b*b) || (b*b+c*c==a*a)) printf ("Trojuhelnik je pravouhly.\n");
	else if (alfa > M_PI/2 || beta > M_PI/2 || gama > M_PI/2) printf ("Trojuhelnik je tupouhly.\n");
	else printf ("Trojuhelnik je ostrouhly.\n");

	printf("Uhel alfa: %.4f rad = %02d s %02d'%05.2f''\n", alfa, (int)alfas, (int)alfam, alfasec);
	printf("Uhel beta: %.4f rad = %02d s %02d'%05.2f''\n", beta, (int)betas, (int)betam, betasec);
	printf("Uhel gama: %.4f rad = %02d s %02d'%05.2f''\n", gama, (int)gamas, (int)gamam, gamasec);
	printf("Obvod: %.4f\n", a+b+c);
	printf("Obsah: %.4f\n", sqrt(((a+b+c)/2)*(((a+b+c)/2)-a)*(((a+b+c)/2)-b)*(((a+b+c)/2)-c)));
	printf("Polomer kruznice vepsane: %.4f\n", ((a+b+c) * tan(alfa/2) * tan(beta/2) * tan(gama/2) /2));
	printf("Polomer kruznice opsane: %.4f\n", a / (2*sin(alfa)));
	printf("Vyska va: %.4f\n", b*sin(gama));
	printf("Vyska vb: %.4f\n", a*sin(gama));
	printf("Vyska vc: %.4f\n", a*sin(beta));

#ifndef __PROGTEST__
  system ( "pause" ); /* toto progtest "nevidi" */
#endif /* __PROGTEST__ */
  return 0;
}
