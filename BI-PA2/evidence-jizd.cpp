#define EQUAL 0
#define LEFT_BIGGER 1
#define RIGHT_BIGGER -1

class CDriver;
class CCar;

class CDriver {
	public:
		CDriver (const string & name, const string & surname );
		~CDriver (void);

    friend ostream& operator<< (ostream& os, const CDriver& driver);

    string GetName(void) const { return m_Name+" "+m_Surname; }
    string GetNameOnly (void) const { return m_Name; }
    string GetSurnameOnly (void) const { return m_Surname; }
    void AddRecord (const string& date, CCar * car);
    int IsSame (const string & val, bool name = true) const;
    bool IsAvailable (const string & date) const;
    void Print(ostream& os) const;

		friend class ::CTester;
		friend class CDatabase;
	protected:
		struct TLog {
			string m_Date;
			CCar * m_Car;
		};
    string m_Name;
    string m_Surname;
    TLog * m_Log;
    int m_LogNr, m_LogMax;
    
    void enlargeArray(void);
    void shiftArray(int idx_from);
    bool searchDate(const string& date, int& pos) const;
};
 
class CCar {
	public:
		CCar (const string & RZ);
		~CCar (void);
		
    friend ostream& operator<< (ostream& os, const CCar& car);
    
    string GetRZ(void) const { return m_RZ; }
    void AddRecord (const string& date, CDriver * driver);
    int IsSame (const string& rz) const;
    bool IsAvailable (const string & date) const;
    void Print(ostream& os) const;
    
		friend class ::CTester;
		friend class CDatabase;
	protected:
		struct TLog {
			string m_Date;
    	CDriver * m_Driver;
		};
    string m_RZ;
    TLog * m_Log;
    int m_LogNr, m_LogMax;
    
    void enlargeArray(void);
    void shiftArray(int idx_from);
    bool searchDate(const string& date, int& pos) const;
}; 

class CDatabase {
	public:
		CDatabase (void); 
		CDatabase (const CDatabase& src);
		~CDatabase (void);

		friend ostream& operator<< (ostream& os, const CDatabase& db);
		
		CDatabase& operator= (const CDatabase& src);
    bool Add (const string & date, const string & name, const string & surname, const string & RZ);
    const CCar & FindCar (const string & RZ) const;
    const CDriver & FindDriver ( const string & name, const string & surname ) const;
    
    friend class ::CTester;
	protected:
    CDriver ** m_Drivers;
    int m_DriversNr, m_DriversMax;
    CCar ** m_Cars;
    int m_CarsNr, m_CarsMax;
    
    void enlargeDrivers(void);
    void enlargeCars(void);
    void shiftCars(int idx_from);
    void shiftDrivers(int idx_from);
    bool searchDriver(const string& name, const string& surname, int& pos) const;
    bool searchCar (const string& RZ, int& pos) const;
    void free(void);
    void copy (const CDatabase& src);
}; 

//operator << overloads --------------------------------------------------------------
//class CDriver
ostream& operator<< (ostream& os, const CDriver& driver) {
	os << driver.m_Name << " " << driver.m_Surname << endl;
	for (int i = 0; i < driver.m_LogNr; i++) os << "   " << driver.m_Log[i].m_Date << ": " << driver.m_Log[i].m_Car->GetRZ() << endl;
	return os;
}
//class CCar
ostream& operator<< (ostream& os, const CCar& car) {
	os << car.m_RZ << endl;
	for (int i = 0; i < car.m_LogNr; i++) {
		os << "   " << car.m_Log[i].m_Date << ": " << car.m_Log[i].m_Driver->GetName() << endl;
	}
	return os;
}
//class CDatabase
ostream& operator<< (ostream& os, const CDatabase& db) {
	os << "Drivers:" << endl;
	for (int i = 0; i < db.m_DriversNr; i++) {
		db.m_Drivers[i]->Print(os);
	}
	os << "Cars:" << endl;
	for (int i = 0; i < db.m_CarsNr; i++) db.m_Cars[i]->Print(os);
	return os;
}

//CDriver methods --------------------------------------------------------------------
CDriver::CDriver (const string & name, const string & surname ) {
	m_Name = name;
	m_Surname = surname;
	m_LogNr = 0;
	m_LogMax = 100;
	m_Log = new TLog[m_LogMax];
}

CDriver::~CDriver (void) {
	delete [] m_Log;
}

int CDriver::IsSame (const string & val, bool name) const {
	if (name == false) {
		if (val == m_Name) return EQUAL;
		else if (val < m_Name) return RIGHT_BIGGER;
		else return LEFT_BIGGER;
	}
	else {
		if (val == m_Surname) return EQUAL;
		else if (val < m_Surname) return RIGHT_BIGGER;
		else return LEFT_BIGGER;
	}
}

bool CDriver::searchDate(const string& date, int& pos) const {
	int idx_from = 0, idx_to = m_LogNr-1;
	while (1) {
		if (idx_from > idx_to) break;
		int middle = (idx_from + idx_to) / 2;
		
		if (m_Log[middle].m_Date == date) {
			pos = middle;
			return true;
		}
		else if (m_Log[middle].m_Date > date) idx_to = middle-1;
		else idx_from = middle + 1;
	}
	pos = idx_from;
	return false;
}

bool CDriver::IsAvailable (const string & date) const {
	int pos;
	if (searchDate(date, pos)) return false;
	return true;
}

void CDriver::enlargeArray (void) {
	m_LogMax *= 2;
	TLog * new_array = new TLog [m_LogMax];
	for (int i = 0; i < m_LogMax; i++) new_array[i] = m_Log[i];
	
	delete [] m_Log;
	m_Log = new_array;
}

void CDriver::shiftArray(int idx_from) {
	if (m_LogNr >= m_LogMax) enlargeArray();
	
	TLog item = m_Log[idx_from];
	for (int i = idx_from; i <= m_LogNr-1; i++) {
		TLog mem = m_Log[i+1];
		m_Log[i+1] = item;
		item = mem;
	}
}

void CDriver::AddRecord (const string& date, CCar * car) {
	int pos;
	if(searchDate(date, pos)) return;
	
	shiftArray(pos);
	m_Log[pos].m_Date = date;
	m_Log[pos].m_Car = car;
	m_LogNr++;
}

void CDriver::Print(ostream& os) const {
	os << "   " << m_Name << " " << m_Surname << endl;
	for (int i = 0; i < m_LogNr; i++) {
		os << "      " << m_Log[i].m_Date << ": " << m_Log[i].m_Car->GetRZ() << endl;
	}
}

//CCar methods --------------------------------------------------------------------
CCar::CCar (const string & rz ) {
	m_RZ = rz;
	m_LogNr = 0;
	m_LogMax = 100;
	m_Log = new TLog[m_LogMax];
}

CCar::~CCar (void) {
	delete [] m_Log;
}

int CCar::IsSame (const string& rz) const {
	if (rz == m_RZ) return EQUAL;
 	else if (rz < m_RZ) return RIGHT_BIGGER;
 	else return LEFT_BIGGER;
}

bool CCar::searchDate(const string& date, int& pos) const {
	int idx_from = 0, idx_to = m_LogNr-1;
	while (1) {
		if (idx_from > idx_to) break;
		int middle = (idx_from + idx_to) / 2;
		
		if (m_Log[middle].m_Date == date) {
			pos = middle;
			return true;
		}
		else if (m_Log[middle].m_Date > date) idx_to = middle-1;
		else idx_from = middle + 1;
	}
	pos = idx_from;
	return false;
}

bool CCar::IsAvailable (const string & date) const {
	int pos;
	if (searchDate(date, pos)) return false;
	return true;
}

void CCar::enlargeArray (void) {
	m_LogMax *= 2;
	TLog * new_array = new TLog [m_LogMax];
	for (int i = 0; i < m_LogMax; i++) new_array[i] = m_Log[i];
	
	delete [] m_Log;
	m_Log = new_array;
}

void CCar::shiftArray(int idx_from) {
	if (m_LogNr >= m_LogMax) enlargeArray();
	
	TLog item = m_Log[idx_from];
	for (int i = idx_from; i <= m_LogNr-1; i++) {
		TLog mem = m_Log[i+1];
		m_Log[i+1] = item;
		item = mem;
	}
}

void CCar::AddRecord (const string& date, CDriver * driver) {
	int pos;
	if(searchDate(date, pos)) return;

	shiftArray(pos);
	m_Log[pos].m_Date = date;
	m_Log[pos].m_Driver = driver;
	m_LogNr++;
}

void CCar::Print(ostream& os) const {
	os << "   " << m_RZ << endl;
	for (int i = 0; i < m_LogNr; i++) {
		os << "      " << m_Log[i].m_Date << ": " << m_Log[i].m_Driver->GetName() << endl;
	}
}

//CDatabase methods ----------------------------------------------------------------
CDatabase::CDatabase (void) {
	m_DriversNr = 0;
	m_CarsNr = 0;
	m_CarsMax = 100;
	m_DriversMax = 100;
	m_Cars = new CCar*[m_CarsMax];
	m_Drivers = new CDriver*[m_DriversMax];
}

CDatabase::CDatabase (const CDatabase& src) {
	copy(src);
}

CDatabase::~CDatabase (void) {
	free();
}
bool CDatabase::searchDriver(const string & name, const string & surname, int & pos) const {
	int idx_from = 0, idx_to = m_DriversNr-1;
	while (1) {
		if (idx_from > idx_to) break;
	
		int middle = (idx_from + idx_to) / 2;
		int cmp = m_Drivers[middle]->IsSame(surname);
		
		if (cmp == EQUAL) {
			int cmp2 = m_Drivers[middle]->IsSame(name, false);
			if (cmp2 == EQUAL) {
				pos = middle;
				return true;
			}
			else if (cmp2 == RIGHT_BIGGER) {
				idx_to = middle - 1;
				continue;
			}
			else {
				idx_from = middle + 1;
				continue;
			}			
		}
		else if (cmp == RIGHT_BIGGER) idx_to = middle-1;
		else idx_from = middle + 1;
	}
	pos = idx_from;
	return false;
}

bool CDatabase::searchCar(const string & rz, int & pos) const {
	int idx_from = 0, idx_to = m_CarsNr-1;
	while (1) {
		if (idx_from > idx_to) break;
		int middle = (idx_from + idx_to) / 2;
		int cmp = m_Cars[middle]->IsSame(rz);
		
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

const CCar & CDatabase::FindCar (const string & RZ) const {
	int pos;
	bool success;
	success = searchCar(RZ, pos);
	if (!success) throw InvalidCarException(RZ);
	return *m_Cars[pos];
} 
const CDriver & CDatabase::FindDriver ( const string & name, const string & surname ) const {
	int pos;
	bool success;
	success = searchDriver(name, surname, pos);
	if (!success) throw InvalidDriverException(name, surname);
	return *m_Drivers[pos];
}

bool CDatabase::Add (const string & date, const string & name, const string & surname, const string & RZ) {
	int pos_driver, pos_car;
	bool exist_driver, exist_car;
	exist_driver = searchDriver (name, surname, pos_driver);
	exist_car = searchCar (RZ, pos_car);
	
	if (exist_driver && !m_Drivers[pos_driver]->IsAvailable(date)) return false; //determine whether the driver is available on 'date'
	if (exist_car && !m_Cars[pos_car]->IsAvailable(date)) return false; //determine whether the car is available on 'date'
	
	if(!exist_driver) {
		shiftDrivers(pos_driver); //shift existing drivers from 'pos_driver' index
		m_Drivers[pos_driver] = new CDriver (name, surname); //add driver to desired position
		m_DriversNr++;
	}
	if (!exist_car) {
		shiftCars(pos_car); //shift existing cars from 'pos_car' index
		m_Cars[pos_car] = new CCar (RZ); //add car to desired position
		m_CarsNr++;
	}

	m_Drivers[pos_driver]->AddRecord(date, m_Cars[pos_car]);
	m_Cars[pos_car]->AddRecord(date, m_Drivers[pos_driver]);
	
	return true;
}

void CDatabase::shiftCars (int idx_from) {
	if (m_CarsNr >= m_CarsMax) enlargeCars(); //if array is full, enlarge it
	
	CCar * item = m_Cars[idx_from]; //item to start shifting
	for (int i = idx_from; i < m_CarsNr; i++) {
		CCar * mem = m_Cars[i+1]; //remember target value
		m_Cars[i+1] = item; //insert new value
		item = mem; //set remembered value to be shifted in next iteration
	}
}

void CDatabase::shiftDrivers (int idx_from) {
	if (m_DriversNr >= m_DriversMax) enlargeDrivers(); //if array is full, enlarge it
	
	CDriver * item = m_Drivers[idx_from]; //item to start shifting
	for (int i = idx_from; i < m_DriversNr; i++) {
		CDriver * mem = m_Drivers[i+1]; //remember target value
		m_Drivers[i+1] = item; //insert new value
		item = mem; //set remembered value to be shifted in next iteration
	}
}

void CDatabase::enlargeCars (void) {
	m_CarsMax *= 2;
	CCar ** new_array = new CCar*[m_CarsMax];
	for (int i = 0; i < m_CarsNr; i++) m_Cars[i] = new_array[i];

	delete [] m_Cars;
	m_Cars = new_array;
}

void CDatabase::enlargeDrivers (void) {
	m_DriversMax *= 2;
	CDriver ** new_array = new CDriver*[m_DriversMax];
	for (int i = 0; i < m_DriversNr; i++) m_Drivers[i] = new_array[i];

	delete [] m_Drivers;
	m_Drivers = new_array;
}

void CDatabase::free (void) {
	for (int i=0; i < m_CarsNr; i++) {
		delete m_Cars[i];
	}
	for (int i=0; i < m_DriversNr; i++) {
		delete m_Drivers[i];
	}
	delete [] m_Cars;
	delete [] m_Drivers;
}

CDatabase& CDatabase::operator= (const CDatabase& src) {
	if (&src == this) return *this;
	free();
	
	copy(src);
	
	return *this;
}

void CDatabase::copy (const CDatabase& src) {
	m_DriversNr = 0;
	m_CarsNr = 0;
	m_CarsMax = src.m_CarsMax;
	m_DriversMax = src.m_DriversMax;
	m_Cars = new CCar*[m_CarsMax];
	m_Drivers = new CDriver*[m_DriversMax];

	for (int i = 0; i < src.m_DriversNr; i++) {
		string name = src.m_Drivers[i]->GetNameOnly();
		string surname = src.m_Drivers[i]->GetSurnameOnly();
		for (int j = 0; j < src.m_Drivers[i]->m_LogNr; j++) {
			string RZ = src.m_Drivers[i]->m_Log[j].m_Car->GetRZ();
			string date = src.m_Drivers[i]->m_Log[j].m_Date;
			Add (date, name, surname, RZ);
		}
	}
}
