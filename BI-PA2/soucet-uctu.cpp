//Author: Jakub Olejnik
//jakub.olejnik@email.cz
#ifndef  __PROGTEST__ 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cctype>
using namespace std;
#endif /* __PROGTEST__ */

void nextSpace (istream& input) {
	char getChar = input.get();
	while (getChar != ' ' && getChar != '\n' && input.good() )  {
		getChar = input.get();
	}
	input.seekg((int)input.tellg()-1);
}

bool isNumber(char c) {
	return (c >= '0' && c <= '9');
}

string nextToken (istream& input, bool& endln) { //nacte dalsi token z input a pokud se ukonci znakem \n tak nastavi priznak endln => ve spravnem vstupu nelze nacist samotny znak '\n'bez toho aby nebyly nastaveny predchozi tokeny
	char getChar = input.get();
	string token = "";
	bool dot_present = false;

	while (getChar == '0') getChar = input.get();
	while (getChar != ' ' && getChar != '\n' && input.good()) {
//		cout << getChar << " dotpresent: " << dot_present << endl;
		if (getChar == 'x') {
			if (token.length() > 0) {
				nextSpace(input);
				break;
			}
			else if(input.get() != ' ') break;
			else return "x";
		}
		else if (getChar == '.' && dot_present) {
			nextSpace(input);
			break;
		}
		else if (!isNumber (getChar) && getChar != '.' && getChar != 'x') {
			nextSpace(input);
			break;
		}
		else {
			if (getChar == '.') dot_present = true;
			token += getChar;
		}
		getChar = input.get();
	}
	if (getChar == '\n') endln = true;
	if (token.length() && input.eof()) endln = true;
	return token;
}

bool sumBill (const char * fileName, double& sum ) {
	ifstream file (fileName);
	double p_sum = 0;
	
	if (!file.is_open()) return false;
	
	int cislo1 = -1; //vstupni cisla nesmi byt zaporna, tak pomoci nich detekuji nenastavenou hodnotu
	bool pocet = false; //detekuje zda se bude cislo2 nasobit cislem 1
	bool name = false; //detekuje zda polozka ma nazev
	bool name3 = false; //detekuje zda lze pouzit zapis pocet x cena
	int pruchody = 0;
	
	while(!file.eof()) {
//		cout << "suma: " << p_sum << endl; 
		if(pruchody == 2) name3 = true;
		bool endln = false;
		string token = nextToken(file, endln);
		cout << "token: >" << token << "< endln:" << endln << endl;
		if (endln) { //pokud jsem nacetl token, ktery se ukoncil novym radkem
			if (!pruchody || token == "" || token == "x" || !name) return false;
//			if (pruchody == 0) return false;
			double cislo2 = atof(token.c_str());
//			cout << "name3: " << name3 << " pocet: " << pocet << endl;
			if (cislo2 <= 0) return false;
			
			if (name3 && pocet) {
				p_sum += cislo2 * cislo1;
				pocet = false;
				name = false;
				name3 = false;
				pruchody = 0;
				continue;
			}
			else if (name) {
				p_sum += cislo2;
				pocet = false;
				name = false;
				name3 = false;
				pruchody = 0;
				continue;
			}
			else return false;			
		}
		else {
			name = true;
			
			if (token == "x" && cislo1 > 0) pocet = true;
			else if (token.find_first_of(".")!= string::npos) {
				pocet = false;
				cislo1 = -1;
			}
			else if (token == "") {pocet = false; cislo1 = -1;}
			else cislo1 = atoi(token.c_str());
			pruchody++;
		}
	}
	sum = p_sum;
	file.close();	
	return true;
}
#ifndef __PROGTEST__
int main () {
	bool res;
double sum;
res = sumBill ( "input1.txt", sum );
cout << "res=" << res << " sum=" << sum << endl;
	return 0;
}
#endif
