#include <iostream>
#include <sstream>

using namespace std;

class CBigInt {
	bool m_Sign; //sign, 0 = +, 1 = -
	string m_Abs; //Abs of stored number
	
	static string numberToString (int number); //converts Abs of number to string
	static void switchVars(int& a, int& b);
	static void switchVars(string& a, string& b);
	friend string diff(const CBigInt& operL, const CBigInt& operR);

	public:
		void removeLeadingZeros();
		CBigInt (int number = 0) { //constructor for int argument
			m_Sign = (number < 0);
			if (number < 0) number = -number;
			m_Abs = numberToString(number);
		}
		CBigInt (const char * number) { //constructor for string/ASCIIZ argument
			m_Sign = (number[0] == '-'); //set sign
			if (m_Sign || number[0] == '+') { //check if argument was passed with sign
				const char * my_number = number;
				m_Abs = my_number+1; //if so, the Abs starts on char
			}	else m_Abs = number;
			removeLeadingZeros ();
		}
		CBigInt operator+= (const CBigInt& add);
		CBigInt operator*= (const CBigInt& add);
		
		
		friend CBigInt operator+ (const CBigInt& left, const CBigInt& right);
		friend CBigInt operator* (const CBigInt& left, const CBigInt& right);
		friend istream& operator>> (istream& is, CBigInt& dst);
		friend ostream& operator<< (ostream& os, CBigInt print);
};

void CBigInt::removeLeadingZeros () {
	const char * str = m_Abs.c_str();
	for (unsigned i = 0; i < m_Abs.length(); i++,str++) if (*str != '0') break;
	m_Abs = str;
	if (m_Abs == "" || m_Abs =="0") { m_Abs = "0"; m_Sign = 0;}
}

string CBigInt::numberToString (int number) { //works with numbers >= 0
	string result = ""; //empty result string initialization
	if (number == 0) return "0";
	while (number > 0) { 
		char numeral = (number % 10) + '0'; //getting last numeral from the number
		result = numeral + result; //prepend numeral to result
		number /= 10; //go on next numeral
	}
	return result;
}

void CBigInt::switchVars (int& a, int& b) {
	int mem = a;
	a = b;
	b = mem;
}

void CBigInt::switchVars (string& a, string& b) {
	string mem = a;
	a = b;
	b = mem;
}

string diff (const CBigInt& operL, const CBigInt& operR) {
	string left = operL.m_Abs;
	string right = operR.m_Abs;
	int length_left, length_right;
	length_left = left.length()-1;
	length_right = right.length()-1;
	bool sgn = operL.m_Sign;
	
	if (length_right > length_left) {
		CBigInt::switchVars (left, right);
		CBigInt::switchVars (length_left, length_right);
		sgn = operR.m_Sign;
	}
	else if (length_right == length_left) {
		for (int i = 0; i < length_right; i++) {
			if (right[i] > left[i]) {
				CBigInt::switchVars(left, right);
				CBigInt::switchVars(length_left, length_right);
				sgn = operR.m_Sign;
				break;
			}
		}
	}
	
	string subresult;
	bool carry = 0;
	
	while (1) {
		int a,b;
		if (length_right < 0 && length_left < 0) {
			break;
		}

		if (length_left < 0) a = 0;
		else a = left[length_left] - '0';		
		
		if (length_right < 0) b = 0;
		else b = right[length_right] - '0';
		b += carry;
		
		if (b > a) {
			a += 10;
			carry = 1;
			char h = a-b + '0';
			subresult = h + subresult;
		}
		else {
			carry = 0;
			char h = a-b + '0';
			subresult = h + subresult;
		}
		length_left--;
		length_right--;
	}
	if (carry) subresult = (char)carry + subresult;
	string result = "";
	result = subresult;
	if (sgn && result != "0") result = '-'+result;
	return result;
}

ostream& operator<< (ostream& os, CBigInt print) { //output stream operator overload
	os << (print.m_Sign == true ? "-" : "") << print.m_Abs;
	return os;
}

CBigInt operator+ (const CBigInt& operandL, const CBigInt& operandR) {
	if ((operandL.m_Sign && !operandR.m_Sign) || (!operandL.m_Sign && operandR.m_Sign)) {
		CBigInt result = diff(operandL, operandR).c_str();
		result.removeLeadingZeros();
		return result;
	}
	else {
		bool carry = 0;
		bool sign = operandL.m_Sign;
		string left = operandL.m_Abs;
		string right = operandR.m_Abs;
		int length_left = left.length()-1;
		int length_right = right.length()-1;
		
		string result = "";
		
		while (length_left >= 0 || length_right >= 0) {
			int char_a, char_b;
			if (length_left < 0) char_a = 0;
			else char_a = left[length_left] - '0';
			
			if (length_right < 0) char_b = 0;
			else char_b = right[length_right] - '0';

			int res = char_a + char_b + carry;
			if(res > 9)	{
				result = CBigInt::numberToString(res-10) + result;
				carry = 1;
			} else {
				carry = 0;
				result = CBigInt::numberToString(res) + result;
			}
			length_left--;
			length_right--;
		}
		
		if (carry) result = '1'+result;
		if (sign && result != "0") result = '-'+result;
	
	CBigInt r = result.c_str();
	r.removeLeadingZeros();
	return r;
	}
}

CBigInt operator* (const CBigInt& left, const CBigInt& right) {
	bool sgn = left.m_Sign ^ right.m_Sign;
	
	CBigInt result;
	int i,k;
	for (i = right.m_Abs.length()-1, k = 0; i >= 0 ; i--,k++) {
		int carry = 0;
		int number = right.m_Abs[i] - '0';
		string res = "";
		for (int j = left.m_Abs.length()-1; j >= 0; j--) {
			int number2 = left.m_Abs[j] - '0';
			int subres = number * number2 + carry	;
			carry = subres / 10;
			res = CBigInt::numberToString(subres%10) + res;
		}
		for(int l = 0; l < k; l++) res += '0';
		if (carry) res = CBigInt::numberToString(carry) + res;
		result = result + CBigInt(res.c_str());
	}
	result.m_Sign = sgn;
	result.removeLeadingZeros();
	return result;
}

CBigInt CBigInt::operator+= (const CBigInt& add) {
	CBigInt bigInt;
	
	bigInt = *this + add;
	m_Sign = bigInt.m_Sign;
	m_Abs = bigInt.m_Abs;
	return *this;
}

CBigInt CBigInt::operator*= (const CBigInt& add) {
	CBigInt bigInt(*this*add);
	m_Sign = bigInt.m_Sign;
	m_Abs = bigInt.m_Abs;
	return *this;
}

istream& operator>> (istream& is, CBigInt& dst) {
	string input = "";
	bool sgn = false;
	while (isspace(is.peek())) is.get(); //skip whitespaces
	if (is.peek()=='-') { is.get(); sgn = true; }
	while (is.good()) {
		if (isdigit(is.peek())) input += is.get();
		else break;
	}
	if (input == "") is.setstate(ios::failbit);
	else {
		dst.m_Abs = input;
		dst.m_Sign = sgn;
	}
	return is;
}

int main()
{
CBigInt       a=1,b;

for (int i = 2; i <= 5000; i++) {
	a *= i;
	cout << i << endl;
}
cout << "------------------------------" << endl;
cout << a << endl;
return 0;
/*

istringstream is;

a = 10;
a += 20;  // a = 30 
cout << a << endl;
a *= 5;  // a = 150 
cout << a << endl;
b = a + 3;  // b = 153 
cout << b << endl;
b = a * 7;  // b = 1050 
cout << b << endl;
cout << a << endl;  // 150

a = 10;
a += -20;  // a = -10 
cout << a << endl;
a *= 5;  // a = -50 
cout << a << endl;
b = a + 73;  // b = 23 
cout << b << endl;
b = a * -7;  // b = 350 
cout << b << endl;
cout << a << endl;  // -50

a = "12345678901234567890";
a += "-99999999999999999999";  // a = -87654321098765432109 
cout << a << endl;
a *= "54321987654321987654";  // a = -4761556948575111126880627366067073182286 
cout << a << endl;
a *= 0;  // a = 0 
cout << a << endl;
a = 10;
b = a + "400";  // b = 410 
cout << b << endl;
b = a * "15";  // b = 150 
cout << b << endl;
cout << a << endl;  // 10

is . clear ();
is . str ( " 1234" );
is >> b; // b = 1234, is . fail() = 0
cout << b << " " << is.fail() << endl;
is . clear ();
is . str ( " 12 34" );
is >> b; // b = 12, is . fail() = 0
cout << b << " " << is.fail() << endl;
is . clear ();
is . str ( "999z" );
is >> b; // b = 999, is . fail() = 0
cout << b << " " << is.fail() << endl;
is . clear ();
is . str ( "abcd" );
is >> b; // b = 999, is . fail() = 1
cout << b << " " << is.fail() << endl;
return 0;*/

}


