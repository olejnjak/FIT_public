%{
#define _WORD 1
#define _WORDEND 2
#define _WS 3

#define _STO_LOWER 1
#define _SFIRST_CAP 2

int state = _SFIRST_CAP;

char * to_low (char * s) {
	int len = strlen (s), i;
	for (i = 0; i < len; i++)
		if (s[i] >= 'A' && s[i] <= 'Z') s[i] = 'a' + s[i] - 'A';
	return s;
}
%}

word		([^\n\t \.\?]+)
wordend ([^\n\t \.\?]+[\.\?!][\t\n ]*)
ws			([\n\t ]+)

%%
{word}	{ if (state == _STO_LOWER) printf("%s", to_low(yytext));
					else if (state == _SFIRST_CAP) {
						char c = yytext[0];
						char * s = to_low(yytext);
						s[0] = c;
						printf("%s", s);
					}
					return _WORD; }

{wordend}	{ if (state == _STO_LOWER) printf("%s", to_low(yytext));
						else if (state == _SFIRST_CAP) {
							char c = yytext[0];
							char * s = to_low(yytext);
							s[0] = c;
							printf("%s", s);
						}
						return _WORDEND; }
						
{ws}		{	printf("%s", yytext); return _WS; }
%%

int main () {
	int res;
	
	while (res = yylex()) {
		switch (res) {
			case _WORD: state = _STO_LOWER; break;
			case _WORDEND: state = _SFIRST_CAP; break;
		}
	}
	return 0;
}
