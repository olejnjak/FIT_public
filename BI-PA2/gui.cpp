#define INITIAL_SIZE 20

class CControl;
class CButton;
class CInput;
class CLabel;
class CComboBox;
class CWindow;


// CWindow header ------------------------------------------------------------------------------------------------------------------------------------------------
class CWindow {
	friend class CControl;

	CControl ** m_Controls; //array of stored controls
	unsigned m_ControlsCount, m_ControlsCountMax;
	int m_X, m_Y, m_Width, m_Height;
	string m_Title;
	
	void enlargeArray(void);
	void copy (const CWindow& src);
	void free (void);
	
	public:
		CWindow (string title, int x, int y, int w, int h);
		CWindow (const CWindow& src);
		~CWindow (void);
		CWindow& Add (const CControl& x);
		void SetPosition (int x, int y, int w, int h);
		CControl * Search (int id) const;
		
		friend ostream& operator<< (ostream& os, const CWindow& print);
		CWindow& operator= (const CWindow& src);
};

// CControl header -----------------------------------------------------------------------------------------------------------------------------------------------
class CControl {
	protected:
		int m_Id;
		double m_rX, m_rY, m_rWidth, m_rHeight; //relative position
		int m_aX, m_aY, m_aWidth, m_aHeight; //absolute position
		string m_Text;
	
	public:
		CControl (int id, double x, double y, double w, double h, string text = "");
		virtual ~CControl (void) {}
		void CountAbsolutePos (CWindow * window); //recounts relative position to absolute
		virtual void Print (ostream& os, bool plus = false, bool last = false) const = 0;
		friend ostream& operator<< (ostream& os, const CControl& print);
		virtual CControl * Copy (void) const = 0;
		bool operator== (int x) const;
};

// CButton header ------------------------------------------------------------------------------------------------------------------------------------------------
class CButton : public CControl {
	public:
		CButton (int id, double x, double y, double w, double h, string text = ""); //remember calling CControl constructor!
		void Print (ostream& os, bool plus = false, bool last = false) const;
		CControl * Copy (void) const;
};

// CLabel header -------------------------------------------------------------------------------------------------------------------------------------------------
class CLabel : public CControl {
	public:
		CLabel (int id, double x, double y, double w, double h, string text = ""); //remember calling CControl constructor!
		void Print (ostream& os, bool plus = false, bool last = false) const;
		CControl * Copy (void) const;
};

// CInput header -------------------------------------------------------------------------------------------------------------------------------------------------
class CInput : public CControl {
	public:
		CInput (int id, double x, double y, double w, double h, string text = ""); //remember calling CControl constructor!
		void SetValue (string x);
		string GetValue (void) const;
		void Print (ostream& os, bool plus = false, bool last = false) const;
		CControl * Copy (void) const;
};

// CComboBox header ----------------------------------------------------------------------------------------------------------------------------------------------
class CComboBox : public CControl {
	unsigned m_Selected;
	string * m_Items;
	unsigned m_ItemsCount, m_ItemsCountMax;
	
	void enlargeArray (void);
	
	public:
		CComboBox (int id, double x, double y, double w, double h); //remember calling CControl constructor!
		CComboBox (const CComboBox& src);
		~CComboBox (void);
		CComboBox& Add (const string& x);
		void SetSelected (int idx = 0);
		int GetSelected (void) const;	
		void Print (ostream& os, bool plus = false, bool last = false) const;
		CControl * Copy (void) const;
};

// CWindow -------------------------------------------------------------------------------------------------------------------------------------------------------
CWindow::CWindow (string title, int x, int y, int w, int h) {
	m_Title = title;
	m_X = x;
	m_Y = y;
	m_Width = w;
	m_Height = h;
	m_ControlsCount = 0;
	m_ControlsCountMax = INITIAL_SIZE;
	m_Controls = new CControl*[INITIAL_SIZE];
}

CWindow::CWindow (const CWindow& src) {
	copy(src);
}

void CWindow::copy (const CWindow& src) {
	m_ControlsCountMax = src.m_ControlsCountMax;
	m_ControlsCount = src.m_ControlsCount;
	m_Title = src.m_Title;
	m_X = src.m_X;
	m_Y = src.m_Y;
	m_Width = src.m_Width;
	m_Height = src.m_Height;
	
	m_Controls = new CControl*[m_ControlsCountMax];
	for(unsigned i = 0; i < m_ControlsCount; i++) m_Controls[i] = src.m_Controls[i]->Copy();
}

void CWindow::free (void) {
	for (unsigned i = 0; i < m_ControlsCount; i++) delete m_Controls[i];
	delete [] m_Controls;
}

CWindow::~CWindow (void) {
	free ();
}

CWindow& CWindow::operator= (const CWindow& src)  {
	free();
	copy(src);
	return *this;
}

void CWindow::SetPosition (int x, int y, int w, int h) {
	m_X = x;
	m_Y = y;
	m_Width = w;
	m_Height = h;
	
	for (unsigned i = 0; i < m_ControlsCount; i++) m_Controls[i]->CountAbsolutePos(this);
}

CWindow& CWindow::Add (const CControl& x) {
	if (m_ControlsCount >= m_ControlsCountMax) enlargeArray();
	CControl * add = x.Copy();
	m_Controls[m_ControlsCount++] = add;
	add->CountAbsolutePos(this);
	return *this;
}

CControl * CWindow::Search (int id) const {
	for (unsigned i = 0; i < m_ControlsCount; i++) {
		if (*m_Controls[i] == id) return m_Controls[i];
	}
	return NULL;
}

ostream& operator<< (ostream& os, const CWindow& print) {
	os << "Window \"" << print.m_Title << "\"" << " (" << print.m_X << "," << print.m_Y << "," << print.m_Width << "," << print.m_Height << ")" << endl;
	for (unsigned i=0; i < print.m_ControlsCount; i++ ) {
		if (i == print.m_ControlsCount-1) print.m_Controls[i]->Print(os, true, true);
		else print.m_Controls[i]->Print(os, true, false);
	}
	return os;
}

void CWindow::enlargeArray (void) {
	CControl ** new_array = new CControl*[m_ControlsCountMax * 2];
	for (unsigned i = 0; i < m_ControlsCount; i++) new_array[i] = m_Controls[i];
	m_ControlsCountMax *= 2;
	delete [] m_Controls;
	m_Controls = new_array;
}

// CControl ------------------------------------------------------------------------------------------------------------------------------------------------------
CControl::CControl (int id, double x, double y, double w, double h, string text) : m_Text (text) {
	m_Id = id;
	m_rX = x;
	m_rY = y;
	m_rWidth = w;
	m_rHeight = h;
}

void CControl::CountAbsolutePos (CWindow * window) {
	m_aWidth = window->m_Width * m_rWidth;
	m_aHeight = window->m_Height * m_rHeight;
	m_aX = window->m_Width * m_rX + window->m_X;
	m_aY = window->m_Height * m_rY + window->m_Y;
}

ostream& operator<< (ostream& os, const CControl& print) {
	print.Print(os, false, true);
	return os;
}

bool CControl::operator== (int id) const {
	return m_Id == id;
}

// CLabel --------------------------------------------------------------------------------------------------------------------------------------------------------
CLabel::CLabel (int id, double x, double y, double w, double h, string text) : CControl (id, x, y, w, h, text) {
}

void CLabel::Print (ostream & os, bool plus, bool last) const {
	if (plus) os << "+- ";
	os << "[" << m_Id << "] Label " << "\"" << m_Text << "\" (" << m_aX << "," << m_aY << "," << m_aWidth << "," << m_aHeight << ")" << endl;
}

CControl * CLabel::Copy (void) const {
	return new CLabel (*this);
}

// CButton -------------------------------------------------------------------------------------------------------------------------------------------------------
CButton::CButton (int id, double x, double y, double w, double h, string text) : CControl (id, x, y, w, h, text) {
}

void CButton::Print (ostream & os, bool plus, bool last) const {
	if (plus) os << "+- ";
	os << "[" << m_Id << "] Button " << "\"" << m_Text << "\" (" << m_aX << "," << m_aY << "," << m_aWidth << "," << m_aHeight << ")" << endl;
}

CControl * CButton::Copy (void) const {
	return new CButton (*this);
}

// CInput -------------------------------------------------------------------------------------------------------------------------------------------------------
CInput::CInput (int id, double x, double y, double w, double h, string text) : CControl (id, x, y, w, h, text) {
}

void CInput::Print (ostream & os, bool plus, bool last) const {
	if (plus) os << "+- ";
	os << "[" << m_Id << "] Input " << "\"" << m_Text << "\" (" << m_aX << "," << m_aY << "," << m_aWidth << "," << m_aHeight << ")" << endl;
}

CControl * CInput::Copy (void) const {
	return new CInput (*this);
}

void CInput::SetValue (string x) {
	m_Text = x;	
}

string CInput::GetValue (void) const {
	return m_Text.c_str();
}

// CComboBox -------------------------------------------------------------------------------------------------------------------------------------------------------
CComboBox::CComboBox (int id, double x, double y, double w, double h) : CControl (id, x, y, w, h) {
	m_Items = NULL;
	m_ItemsCount = 0;
	m_ItemsCountMax = 0;
	m_Selected = 0;
}

CComboBox::CComboBox (const CComboBox& src) : CControl(src) {
	m_Items = new string[src.m_ItemsCountMax];
	m_ItemsCountMax = src.m_ItemsCountMax;
	m_ItemsCount = src.m_ItemsCount;
	m_Selected = src.m_Selected;
	for (unsigned i = 0; i < m_ItemsCount; i++) m_Items[i] = src.m_Items[i];
}

CComboBox::~CComboBox (void) {
	delete [] m_Items;
}

void CComboBox::Print (ostream & os, bool plus, bool last) const {
	if (plus) {
		os << "+- ";
		os << "[" << m_Id << "] ComboBox " << "(" << m_aX << "," << m_aY << "," << m_aWidth << "," << m_aHeight << ")" << endl;
		for (unsigned i = 0; i < m_ItemsCount; i++) {
			if (last) os << " ";
			else os << "|";
			if (i == m_Selected) os << "  +->" << m_Items[i].c_str() << "<" << endl;
			else os << "  +- " << m_Items[i].c_str() << endl;
		}
	}
	else {
		os << "[" << m_Id << "] ComboBox " << "(" << m_aX << "," << m_aY << "," << m_aWidth << "," << m_aHeight << ")" << endl;
		for (unsigned i = 0; i < m_ItemsCount; i++) {
			if (i == m_Selected) os << "+->" << m_Items[i].c_str() << "<" << endl;
			else os << "+- " << m_Items[i].c_str() << endl;
		}
	}
}

CControl * CComboBox::Copy (void) const {
	return new CComboBox (*this);
}

void CComboBox::enlargeArray (void) {
	if (!m_ItemsCountMax) {
		m_ItemsCountMax = 10;
		m_Items = new string[m_ItemsCountMax];
	} else {
		string * new_array = new string[m_ItemsCountMax * 2];
		for (unsigned i = 0; i < m_ItemsCount; i++) new_array[i] = m_Items[i];
		m_ItemsCountMax *= 2;
		delete [] m_Items;
		m_Items = new_array;
	}		
}

CComboBox& CComboBox::Add (const string& x) {
	if (m_ItemsCount >= m_ItemsCountMax) enlargeArray();
	m_Items[m_ItemsCount] = x;
	m_ItemsCount++;

	return *this;
}

void CComboBox::SetSelected(int idx) {
	m_Selected = idx;
}
