#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;
#endif /* __PROGTEST__ */

struct SUser {
	struct SPhone {
		string m_Phone;
		SUser * m_Owner;
		bool m_Valid;
		
		SPhone (string phone, SUser * owner) {
			m_Phone = phone;
			m_Owner = owner;
			m_Valid = true;
		}
		
		int IsSame (const string & phone) {
			if (phone == m_Phone) return 0;
			else if (phone < m_Phone) return -1;
			else return 1;
		}
	};

	string m_Name;
	string m_Address;
	short m_PhonesCount;
	SPhone ** m_Phones;
	int m_Idx;
	

	SUser(const string & name, const string & address, int size, int idx) {
		m_Name = name;
		m_Address = address;
		m_Phones = new SPhone*[size];
		m_PhonesCount = 1; //uzivatele vytvarim kdyz mu poprve prirazuji telefon
		m_Idx = idx;
	}
	
	~SUser() {
//		for (int i = 0; i < m_PhonesCount; i++ ) delete [] m_Phones[i];
		delete [] m_Phones;
		m_Phones = NULL;
	}
	
	int IsSame (const string & val, bool name = true) {
		if (name == true) {
//			cout << "IsSame " << m_Name << "==" << val << endl;
			if (val == m_Name) return 0;
			else if (val < m_Name) return -1;
			else return 1;
		}
		else {
//			cout << "IsSame " << m_Address << "==" << val << endl;
			if (val == m_Address) return 0;
			else if (val < m_Address) return -1;
			else return 1;
		}
	}	
};

class CPhoneBook {
	SUser ** m_Array;
	SUser::SPhone ** m_ArrayPhones;
	int m_ArraySize, m_ArrayPhonesSize;
	int m_ArrayMaxSize, m_ArrayPhonesMaxSize;
	
	void enlargeArray();
	bool searchBinary(const string & name, const string & address, int & pos) const;
	bool searchBinary(const string & phone, int & pos) const;
	
	public:
		static const int MAX_PHONES_PER_USER = 16;
		CPhoneBook    ( void ) { 	
			const int size = 100;
			m_Array = new SUser*[size];
			m_ArraySize = 0;
			m_ArrayMaxSize = size;
			m_ArrayPhones = new SUser::SPhone*[size*MAX_PHONES_PER_USER];
			m_ArrayPhonesSize = 0;
			m_ArrayPhonesMaxSize = size*MAX_PHONES_PER_USER;
		 }
		~CPhoneBook    ( void ) { 
			for (int i = 0; i < m_ArraySize; i++) delete m_Array[i];
			delete [] m_Array;
			for (int i = 0; i < m_ArrayPhonesSize; i++) {
				delete m_ArrayPhones[i];
				m_ArrayPhones[i] = NULL;
			}
			delete [] m_ArrayPhones;
		}
		bool Add (const string & name, const string & address, const string & phone);
		bool Del (const string & name, const string & address);
		bool Del (const string & phone);
		bool Search (const string & name, const string & address, int idx, string & phone) const;
		bool Search (const string & phone, string & name, string & address) const;
};

void CPhoneBook::enlargeArray() {
	m_ArrayMaxSize *= 2;
	SUser ** new_Array = new SUser*[m_ArrayMaxSize];
	for (int i = 0; i < m_ArraySize; i++) new_Array[i] = m_Array[i];
	delete [] m_Array;
	m_Array = new_Array;
	
	m_ArrayPhonesMaxSize *=2;
	SUser::SPhone ** new_ArrayPhones = new SUser::SPhone*[m_ArrayPhonesMaxSize];
	for (int i = 0; i < m_ArrayPhonesSize; i++) new_ArrayPhones[i] = m_ArrayPhones[i];
	delete [] m_ArrayPhones;
	m_Array = new_Array;
}

bool CPhoneBook::searchBinary(const string & name, const string & address, int & pos) const {
	int idx_from = 0, idx_to = m_ArraySize-1;
	while (1) {
		if (idx_from > idx_to) break;
	
		int middle = (idx_from + idx_to) / 2;
		int cmp = m_Array[middle]->IsSame(name);
		
		if (cmp == 0) {
			int cmp2 = m_Array[middle]->IsSame(address, false);
			if (cmp2 == 0) {
				pos = middle;
				return true;
			}
			else if (cmp2 == -1) { //adresa je mensi
				idx_to = middle - 1;
				continue;
			}
			else {
				idx_from = middle + 1;
				continue;
			}			
		}
		else if (cmp == -1) idx_to = middle-1;
		else idx_from = middle + 1;
	}
	pos = idx_from;
	return false;
}

bool CPhoneBook::searchBinary(const string & phone, int & pos) const {
	int idx_from = 0, idx_to = m_ArrayPhonesSize-1;
	while (1) {
		if (idx_from > idx_to) break;
		int middle = (idx_from + idx_to) / 2;
		int cmp = m_ArrayPhones[middle]->IsSame(phone);
		
		if (cmp == 0) {
			pos = middle;
			return true;
		}
		else if (cmp == -1) idx_to = middle-1;
		else idx_from = middle + 1;
	}
	pos = idx_from;
	return false;
}

bool CPhoneBook::Add (const string & name, const string & address, const string & phone) {
	int pos_phone;
	if (searchBinary(phone, pos_phone)) return false; //pokud telefon jiz existuje vratim false
	
	int pos_user;
	SUser::SPhone * new_phone;

	if (searchBinary (name, address, pos_user)) { //pokud uz uzivatel v databazi je
		if (m_Array[pos_user]->m_PhonesCount >= MAX_PHONES_PER_USER) return false;

		SUser * user = m_Array[pos_user];
		new_phone = new SUser::SPhone(phone, user);
		SUser::SPhone * add = new_phone;
		int i;
		for (i = 0; i < user->m_PhonesCount; i++)	if (user->m_Phones[i]->IsSame(phone) == -1) break;
		for (; i < user->m_PhonesCount; i++) {
			SUser::SPhone * item = user->m_Phones[i];
			user->m_Phones[i] = new_phone;
			new_phone = item;
		}
		user->m_Phones[user->m_PhonesCount++] = new_phone;
		new_phone = add;
	}
	else { //uzivatel v databazi neni
		if (m_ArraySize == m_ArrayMaxSize) enlargeArray(); //zajisti roztahnuti obou poli;
		
		SUser * new_user = new SUser(name, address, MAX_PHONES_PER_USER, pos_user); //vytvorim noveho uzivatele

		SUser * add = new_user;
		for (int i = pos_user; i <= m_ArraySize; i++) { //zaradim noveho uzivatele
			SUser * item = m_Array[i];
			m_Array[i] = add;
			add = item;
		}
		new_phone = new SUser::SPhone(phone, new_user);
		new_user->m_Phones[0] = new_phone;
		m_ArraySize++;
	}
	for (int i = pos_phone; i <= m_ArrayPhonesSize; i++) { //zaradim telefon
		SUser::SPhone * item = m_ArrayPhones[i];
		m_ArrayPhones[i] = new_phone;
		new_phone = item;
	}
	m_ArrayPhonesSize++;
	return true;
}

bool CPhoneBook::Search (const string & phone, string & name, string & address) const {
	int pos;
	if (!searchBinary(phone, pos)) return false;
	
	name = m_ArrayPhones[pos]->m_Owner->m_Name;
	address = m_ArrayPhones[pos]->m_Owner->m_Address;
	return true;
}

bool CPhoneBook::Search (const string & name, const string & address, int idx, string & phone) const {
	int pos; 
	if (!searchBinary(name, address, pos)) return false;
	SUser * user = m_Array[pos];
	if (idx >= user->m_PhonesCount) return false;
	
	phone = user->m_Phones[idx]->m_Phone;
	return true;
}

bool CPhoneBook::Del (const string & phone) {
	int pos;
	if (!searchBinary(phone, pos)) return false;

	SUser * owner = m_ArrayPhones[pos]->m_Owner;

	int i;
	for (i = 0; i < owner->m_PhonesCount; i++) {
		if (!owner->m_Phones[i]->IsSame(phone)) break;
	}
	for (; i < owner->m_PhonesCount - 1; i++) owner->m_Phones[i] = owner->m_Phones[i+1];
	owner->m_PhonesCount--;
	
	delete m_ArrayPhones[pos];
	for (int i = pos; i<m_ArrayPhonesSize-1; i++) m_ArrayPhones[i] = m_ArrayPhones[i+1];
	m_ArrayPhonesSize--;
	
	
	if (!owner->m_PhonesCount) {
		int idx = owner->m_Idx;
		delete owner;
		owner = NULL;
		for (int i = idx; i < m_ArraySize-1; i++) m_Array[i] = m_Array[i+1];
		m_ArraySize--;
	}
	return true;
}

bool CPhoneBook::Del (const string & name, const string & address) {
	int pos;
	if (!searchBinary(name, address, pos)) return false;
	
	SUser * del = m_Array[pos];
	
	int phone_idx = 0;
	int pos_phone;
	searchBinary(del->m_Phones[0]->m_Phone, pos_phone);
	int shift = 0;
	for (int i = pos_phone; i < m_ArrayPhonesSize - shift; i++) {
		SUser::SPhone * item = m_ArrayPhones[i+shift];
		if (phone_idx < del->m_PhonesCount) {
			if (item == del->m_Phones[phone_idx]) {
				delete item;
				shift++;
				m_ArrayPhones[i] = m_ArrayPhones[i+shift];
				i--;
				phone_idx++;
				continue;
			}
		}
		m_ArrayPhones[i] = m_ArrayPhones[i+shift];
	}
	m_ArrayPhonesSize -= shift;
	
	delete m_Array[pos];
	for(int i = pos; i < --m_ArraySize; i++) m_Array[i] = m_Array[i+1];
	
	return true;
}

#ifndef __PROGTEST__
int main (int argc, char * argv[]) {
bool   status;
string phone, name, addr;

CPhoneBook b1;
status = b1 . Add ( "Smith", "Oak road", "123456" );
status = b1 . Add ( "Smith", "Oak road", "1234567" );
status = b1 . Add ( "Smith", "Oak road", "1234568" );
status = b1. Search ("123456", name, phone);
status = b1 . Del ( "Smith", "Oak road");
/*
status = b1 . Add ( "Smith", "Oak road", "123456" );
// status = true
status = b1 . Add ( "Brown", "Second street", "7654321" );
// status = true
status = b1 . Add ( "Hacker", "5-th avenue", "02348" );
// status = true
status = b1 . Add ( "Hacker", "7-th avenue", "13278" );
// status = true
status = b1 . Search ( "Brown", "Second street", 0, phone );
// status = true, phone = "7654321"
status = b1 . Search ( "Brown", "Second street", 1, phone );
// status = false
status = b1 . Search ( "Hacker", "Oak road", 0, phone );
// status = false
status = b1 . Search ( "Smith", "Oak road", 0, phone );
// status = true, phone = "123456"
status = b1 . Search ( "Smith", "Oak road", 1, phone );
// status = false
status = b1 . Del ( "Smith", "Oak road" );
// status = true
status = b1 . Search ( "Smith", "Oak road", 0, phone );
// status = false

CPhoneBook  b2;
status = b2 . Add ( "Smith", "Michigan avenue", "9123456" );
// status = true
status = b2 . Search ( "Smith", "Michigan avenue", 0, phone );
// status = true, phone = "9123456"
status = b2 . Search ( "Smith", "Michigan avenue", 1, phone );
// status = false
status = b2 . Del ( "Smith", "Michigan avenue" );
// status = true
status = b2 . Search ( "Smith", "Michigan avenue", 0, phone );
// status = false
status = b2 . Del ( "Smith", "Michigan avenue" );
// status = false
status = b2 . Add ( "Smith", "Michigan avenue", "9123456" );
// status = true
status = b2 . Search ( "Smith", "Michigan avenue", 0, phone );
// status = true, phone = "9123456"
status = b2 . Search ( "Smith", "Michigan avenue", 1, phone );
// status = false
status = b2 . Add ( "Smith", "Michigan avenue", "9123456" );
// status = false
status = b2 . Add ( "Smith", "Michigan avenue", "10293847" );
// status = true
status = b2 . Del ( "Smith", "Michigan avenue" );
// status = true
status = b2 . Search ( "Smith", "Michigan avenue", 0, phone );
// status = false

CPhoneBook  b3;
status = b3 . Add ( "Smith", "Michigan avenue", "1234" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Add ( "Smith", "Michigan avenue", "2233" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Add ( "Smith", "Michigan avenue", "3344" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Add ( "Brown", "Oak road", "5678" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Add ( "Brown", "Oak road", "6677" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Search ( "Smith", "Michigan avenue", 0, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "1234"
status = b3 . Search ( "Smith", "Michigan avenue", 1, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "2233"
status = b3 . Search ( "Smith", "Michigan avenue", 2, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "3344"
status = b3 . Search ( "Smith", "Michigan avenue", 3, phone );
cout << "STATUS: " << status << endl;
// status = false
status = b3 . Search ( "Brown", "Oak road", 0, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "5678"
status = b3 . Search ( "Brown", "Oak road", 1, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "6677"
status = b3 . Search ( "Brown", "Oak road", 2, phone );
cout << "STATUS: " << status << endl;
// status = false
status = b3 . Search ( "2233", name, addr );
cout << "STATUS: " << status << " NAME: " << name << " ADDR: "<< addr<< endl;
// status = true, name = "Smith", addr = "Michigan avenue"
status = b3 . Del ( "6677" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Search ( "Brown", "Oak road", 0, phone );
cout << "STATUS: " << status << " PHONE: " << phone << endl;
// status = true, phone = "5678"
status = b3 . Search ( "Brown", "Oak road", 1, phone );
cout << "STATUS: " << status << endl;
// status = false
status = b3 . Del ( "5678" );
cout << "STATUS: " << status << endl;
// status = true
status = b3 . Search ( "Brown", "Oak road", 0, phone );
cout << "STATUS: " << status << endl;
// status = false*/
	return 0;
}
#endif
