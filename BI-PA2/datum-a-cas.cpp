#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
using namespace std;
class CDateTime {
	long long m_Stamp;
	static const int MINUTE = 60;
	static const int HOUR = MINUTE*60;
	static const int DAY = HOUR*24;
	static const int MONTH = DAY*30;
	static const int YEAR = DAY*365;
	
	static bool parseStamp (string in, int& d, int &m, int& y, int& h, int& min, int& s);
	static bool isLeap (int y);
	static string numberToString (int number); //converts Abs of number to string
	static long long makeStamp (int d, int m, int y, int h, int min, int sec); //makes stamp from input vars, 1.1.1600 0:00 = 0
	static string readStamp (long long stamp); //reads stamp and returns result as a string
	static string read (int count, istream& is);
	static char read (char c, istream& is);
	
	public:
		CDateTime (int y=2000, int m=1, int d=1, int h=0, int mi=0, int s=0);
		CDateTime operator= (string set); //set this to 'set' value
		friend bool operator> (const CDateTime& operL, const CDateTime& operR);
		friend bool operator< (const CDateTime& operL, const CDateTime& operR);
		friend bool operator== (const CDateTime& operL, const CDateTime& operR);
		friend bool operator<= (const CDateTime& operL, const CDateTime& operR);
		friend bool operator>= (const CDateTime& operL, const CDateTime& operR);
		friend bool operator!= (const CDateTime& operL, const CDateTime& operR);
		int operator[] (const char * id) const; //returns wanted part of date
		friend ostream& operator<< (ostream& os, const CDateTime& dt);
		friend istream& operator>> (istream& is, CDateTime& dt);
		friend long long int operator- (const CDateTime& operL, const CDateTime& operR); //return difference in secs between operands
		friend CDateTime operator+ (const CDateTime& operL, const int operR); //create new object and set it to this + operR
};

CDateTime::CDateTime (int y, int m, int d, int h, int mi, int s) {
	m_Stamp = makeStamp(d,m,y,h,mi,s);
}

string CDateTime::read (int count, istream& is){
	if (!is.good()) return "";

	string out = "";
	for (int i = 0; i < count; i++) {
		if (!isdigit(is.peek())) {
			is.setstate(ios::failbit);
			return "";
		}
		out += is.get();
	}
	return out;
}

char CDateTime::read (char c, istream& is) {
	if (!is.good()) return 0;

	if (is.peek() != c) {
		is.setstate(ios::failbit);
		return 0;
	}
	return is.get();
}

int CDateTime::operator[] (const char * id) const {
	string read = readStamp(m_Stamp);
	string idx (id);
	if (idx=="year") {
		char year[5];
		year[0] = read[0];
		year[1] = read[1];
		year[2] = read[2];
		year[3] = read[3];
		year[4] = 0;
		return atoi(year);
	}
	else if (idx == "month") {
		char month [3];
		month[0] = read[5]; 
		month[1] = read[6];
		month[2] = 0;
		return atoi(month);
	}
	else if (idx == "day") {
		char day [3];
		day[0] = read[8]; 
		day[1] = read[9];
		day[2] = 0;
		return atoi(day);
	}
	else if (idx ==  "hour") {
		char hour [3];
		hour[0] = read[11]; 
		hour[1] = read[12];
		hour[2] = 0;
		return atoi(hour);
	}
	else if (idx == "min") {
		char min [3];
		min[0] = read[14]; 
		min[1] = read[15];
		min[2] = 0;
		return atoi(min);
	}
	else {
		char sec [3];
		sec[0] = read[17]; 
		sec[1] = read[18];
		sec[2] = 0;
		return atoi(sec);
	}
}

string CDateTime::numberToString (int number) { //works with numbers >= 0
	string result = ""; //empty result string initialization
	if (number == 0) return "0";
	while (number > 0) { 
		char numeral = (number % 10) + '0'; //getting last numeral from the number
		result = numeral + result; //prepend numeral to result
		number /= 10; //go on next numeral
	}
	return result;
}
bool parseStamp (string in, int& d, int &m, int& y, int& h, int& min, int& s) {
	return true;
}
bool CDateTime::isLeap (int y) {
	short mod4 = y % 4, mod100 = y % 100, mod400 = y % 400;
	if (!mod100 && mod400) return false; //years divided by 100 and not 400 aren't leap
	if (!mod4) return true;
	return false;
}
long long CDateTime::makeStamp (int d, int m, int y, int h, int min, int s) {
	int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (isLeap(y)) days_in_months[1] = 29;

	long long res_stamp = 0;
	for (int i = 1600; i < y; i++) { //increase stamp by previous years
		if(isLeap(i)) res_stamp += YEAR + DAY; //if the year is leap add one day
		else res_stamp += YEAR;
	}

	for (int i = 0; i < m-1; i++) //increase stamp by previous months
		res_stamp += days_in_months[i] * DAY;		
	for (int i = 1; i < d; i++) //increase stamp by previous day in month
		res_stamp += DAY;
	for (int i = 0; i < h; i++) //increase stamp by 'h' hours
		res_stamp += HOUR;
	for (int i = 0; i < min; i++) //increase stamp by 'm' minutes	
		res_stamp += MINUTE;
	res_stamp += s;
	return res_stamp;
}
string CDateTime::readStamp (long long stamp) {
	int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int years=1600, months=1, days=1, hours=0, mins=0, secs=0;

	while (stamp >= YEAR) {
		if (isLeap(years)) stamp -= DAY;
		years++;
		stamp -= YEAR;
	}
	cout << stamp << endl;
	if (stamp < 0) {
		years--;
		days++;
		stamp += YEAR;
	}
	int i = 0;
	if (isLeap(years)) days_in_months[1] = 29;
	while (stamp >= days_in_months[i]*DAY) {
		stamp -= days_in_months[i]*DAY;
		months++;
		i++;
	}
	days += stamp / DAY;
	stamp %= DAY;
	hours = stamp / HOUR;
	stamp %= HOUR;
	mins = stamp / MINUTE;
	stamp %= MINUTE;
	secs = stamp;
	
	string sy(numberToString(years));
	string sm(numberToString(months));
	if (sm.length()==1) sm = '0'+sm;
	string sd(numberToString(days));
	if (sd.length()==1) sd = '0'+sd;
	string sh(numberToString(hours));
	if (sh.length()==1) sh = '0'+sh;
	string smin(numberToString(mins));
	if (smin.length()==1) smin = '0'+smin;
	string s(numberToString(secs));
	if (s.length()==1) s = '0'+s;
	string result(sy+"-"+sm+"-"+sd+" "+sh+":"+smin+":"+s);
	return result;
}

CDateTime CDateTime::operator= (string set) {
	string year = "", month="", day="";
	string hour="", min="", sec="";
	
	year += set[0]; year += set[1]; year += set[2]; year += set[3];
	month += set[5]; month+=set[6];
	day += set[8]; day+=set[9];
	hour+=set[11]; hour+=set[12];
	min+=set[14]; min+=set[15];
	sec+=set[17]; sec+=set[18];
	m_Stamp = makeStamp(atoi(day.c_str()), atoi(month.c_str()), atoi(year.c_str()),atoi(hour.c_str()),atoi(min.c_str()), atoi(sec.c_str()));
	return *this;
}

long long int operator- (const CDateTime& operL, const CDateTime& operR) {
	return (operL.m_Stamp - operR.m_Stamp);
}

CDateTime operator+ (const CDateTime& operL, const int operR) {
	CDateTime a;
	a = operL;
	a.m_Stamp += operR;
	return a;
}

bool operator> (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp > operR.m_Stamp;
}
bool operator< (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp < operR.m_Stamp;
}
bool operator== (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp == operR.m_Stamp;
}
bool operator<= (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp <= operR.m_Stamp;
}
bool operator>= (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp >= operR.m_Stamp;
}
bool operator!= (const CDateTime& operL, const CDateTime& operR) {
	return operL.m_Stamp != operR.m_Stamp;
}

ostream& operator<< (ostream& os, const CDateTime& dt) {
	os << CDateTime::readStamp(dt.m_Stamp) << " " << dt.m_Stamp;
	return os;
}

istream& operator>> (istream& is, CDateTime& dt) {
	int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	string in = "";
	string ret = "";
	int val, month;
	
	ret = CDateTime::read(4, is);
	val = atoi(ret.c_str());
	if (val > 3000 || val < 1600) {
		is.setstate(ios::failbit);
		return is;
	}
	if (CDateTime::isLeap(val)) days_in_months[1] = 29;
	in += ret;
	in += CDateTime::read('-', is);
	
	ret = CDateTime::read(2, is);
	val = atoi(ret.c_str());
	if (val > 12 || val < 1) {
		is.setstate(ios::failbit);
		return is;
	}	
	month = val-1;
	in += ret;
	in += CDateTime::read('-', is);
	
	ret = CDateTime::read(2, is);
	val = atoi(ret.c_str());
	if (val < 1 || val > days_in_months[month]) {
		is.setstate(ios::failbit);
		return is;
	}
	in += ret;	
	in += CDateTime::read(' ', is);
	
	ret = "";
	for (int i = 0; i < 2; i++) {
		if (!isdigit(is.peek())) {
			if (i == 1 && is.peek() == ':') continue;
			is.setstate(ios::failbit);
			return is;
		}
		ret += is.get();
	}
	val = atoi(ret.c_str());
	if (val < 0 || val > 23) {
		is.setstate(ios::failbit);
		return is;
	}
	in += ret;
	in += CDateTime::read(':', is);
	
	ret = CDateTime::read(2, is);
	val = atoi(ret.c_str());
	if (val < 0 || val > 59) {
		is.setstate(ios::failbit);
		return is;
	}
	in += ret;
	in += CDateTime::read(':', is);
	
	ret = CDateTime::read(2, is);
	val = atoi(ret.c_str());
	if (val < 0 || val > 59) {
		is.setstate(ios::failbit);
		return is;
	}
	in += ret;
	
	if(is.good()) dt = in;
	return is;
}

int main () {
	CDateTime a,b(1970);
	cout << "a " << a << endl;
	cout << "b " << b << endl;
	cout << a-b << endl;
	return 0;
}
