template <class T>
class CBST;
template <class T>
class CSparseArray;
template <class T>
ostream & operator << ( ostream & os, const CBST<T> & x );

// strom inspirovany proseminarem------------------------------------------------------

template <class T>
class CBST {
   public:
		CBST ( void );
    CBST ( const CBST<T>   & src );
    ~CBST ( void );
    CBST<T> & operator= ( const CBST<T> & src );
    bool operator== ( const CBST<T> & b ) const;
    bool Insert ( const T& x );
    bool Delete ( const T& x );
    T* IsSet  ( const T& x ) const;
    int Size ( void ) const;

   protected:
    struct TNode {
    	static bool Same (const TNode& a, const TNode& b) {
    		bool equals = a.m_Val.Equals(b.m_Val);
    		bool equals1 = false, equals2 = false;
    		if (!equals) return false;
    		if (!a.m_R && !b.m_R) equals1 = true;
    		else if (a.m_R && b.m_R) equals1 = Same(*a.m_R, *b.m_R);
    		
    		if (!equals1) return false;
    		
    		if (!a.m_L && !b.m_L) equals2 = true;
    		else if (a.m_L && b.m_L) equals2 = Same(*a.m_L, *b.m_L);
    		return (equals && equals1 && equals2 );
    	}
			TNode ( const T& x );
      ~TNode ( void );
      TNode * Clone ( void ) const;
      void Print ( ostream& os, bool first ) const;
      string Print (bool first) const;
      TNode * m_L;
      TNode * m_R;
      T m_Val;
    };
		
		string makeString ( void ) const;
    TNode * m_Root;
    int m_Count;
    friend ostream & operator << <> ( ostream & os, const CBST<T> & x );
};


//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::CBST ( void ) {
	m_Root  = NULL;
	m_Count = 0;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::CBST ( const CBST<T>   & src ) {
	if ( src . m_Root )
		m_Root = src . m_Root -> Clone ();
	else m_Root = NULL;
	m_Count = src . m_Count;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::~CBST ( void ) {
	delete m_Root;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>& CBST<T>::operator= ( const CBST<T>   & src ) {
	if ( this != &src ) {
		delete m_Root;
    if ( src . m_Root )
			m_Root = src . m_Root -> Clone ();
    else m_Root = NULL;
    m_Count = src . m_Count;
	}
  return *this;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::operator== ( const CBST<T>   & b ) const {
	string sa,sb;
	sa = makeString();
	sb = b.makeString();
	cerr << sa << endl;
	cerr << sb << endl;
  return (sa==sb);
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::Insert ( const T& x ) {
	TNode * node = m_Root, * parent = NULL;
	bool left;

	while ( node ) {
		if ( x == node -> m_Val ) {
			node->m_Val = x;
			return true;
		}
		if ( x < node -> m_Val ) {
			parent = node;
			node = node -> m_L;
			left   = true;
		}
		else {
			parent = node;
			node = node -> m_R;
			left = false;
		}
	}
	node = new TNode ( x );
	if ( parent ) {
		if ( left ) parent -> m_L = node;
    else parent -> m_R = node;
	}
  else m_Root = node;
	m_Count ++;
	return true;
}
//-------------------------------------------------------------------------------------------------
template <class T>
bool CBST<T>::Delete ( const T& x ) {
	TNode *node = m_Root, *parent = NULL, *toDel;
	bool left;

	while ( node ) {
		if ( x == node -> m_Val ) { 
			toDel = node;
	    if ( node -> m_L && node -> m_R ) {
				parent = node;
				left = true;
				node = node -> m_L;
				while ( node -> m_R ) {
					parent = node;
					node = node -> m_R;
					left   = false;
				}

				toDel -> m_Val = node -> m_Val;
				toDel = node;
			}

			node = toDel -> m_L ? toDel -> m_L : toDel -> m_R;

			if ( parent ) {
				if ( left ) parent -> m_L = node;
				else parent -> m_R = node;
			}
			else m_Root = node;

			toDel -> m_L = toDel -> m_R = NULL;
			delete toDel;
			m_Count --;
			return true;
		}
		if ( x < node -> m_Val ) {
			parent = node;
			node = node -> m_L;
			left = true;
		}
    else {
			parent = node;
			node = node -> m_R;
			left = false;
		}
	}
  return false;
 }
//-------------------------------------------------------------------------------------------------
template <class T>
T* CBST<T>::IsSet ( const T & x ) const {
	TNode * n = m_Root;

	while ( n ) {
		if ( x == n -> m_Val ) return &n->m_Val;
		if ( x < n -> m_Val ) n = n -> m_L;
    else n = n -> m_R;
	}
	return NULL;
 }
//-------------------------------------------------------------------------------------------------
template <class T>
int CBST<T>::Size ( void ) const {
	return m_Count;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::TNode::TNode ( const T & x ) : m_Val ( x ) {
	m_L = m_R = NULL;
}
//-------------------------------------------------------------------------------------------------
template <class T>
CBST<T>::TNode::~TNode ( void ) {
	delete m_L;
	delete m_R;
}
//-------------------------------------------------------------------------------------------------
template <class T>
typename CBST<T>::TNode * CBST<T>::TNode::Clone ( void ) const {
	TNode  * n;

	n = new TNode ( m_Val );
	if ( m_L ) n -> m_L = m_L -> Clone ();
  if ( m_R ) n -> m_R = m_R -> Clone ();
	return n;
}
//-------------------------------------------------------------------------------------------------
template <class T>
void CBST<T>::TNode::Print ( ostream & os, bool first ) const {
	if ( m_L ) {
		m_L -> Print ( os, first );
		first = false;
  }
	if ( !first ) os << ", ";
	os << m_Val;
	if ( m_R ) m_R -> Print ( os, false );
}
//-------------------------------------------------------------------------------------------------
template <class T>
string CBST<T>::TNode::Print ( bool first ) const {
	istringstream a;
	string ret = "";
	if ( m_L ) {
		ret += m_L -> Print ( first );
		first = false;
  }
	if ( !first ) ret += ", ";
	a << m_Val;
	//ret += m_Val;
	if ( m_R ) ret += m_R -> Print ( false );
	return ret;
}
//-------------------------------------------------------------------------------------------------
template <class T>
ostream& operator<< ( ostream& os, const CBST<T>   & x) {
	os << "{ ";
	if ( x . m_Root ) {
		x . m_Root -> Print ( os, true );
		os << " }";
	} else os << "}";
	return os;
}
//-------------------------------------------------------------------------------------------------
template <class T>
string CBST<T>::makeString ( void ) const {
	string ret = "";
	ret += "{ ";
	if ( m_Root ) {
		ret += m_Root -> Print (true);
		ret += " }";
	} else ret += "}";
	return ret;
}


// konec stromu z proseminare----------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

template <class T>
class CSparseArray;
template <class T>
ostream& operator<< (ostream& os, const CSparseArray<T>& print);
template <class T>
bool operator== (const CSparseArray<T>& a, const CSparseArray<T>& b);
template <class T>
bool operator!= (const CSparseArray<T>& a, const CSparseArray<T>& b);

template <class T>
class CSparseArray {
	struct TElem;
	friend class CBST<TElem>;	
	CBST<TElem> * m_Tree;
	
	struct TElem {
		T m_Val;
		int m_Idx;
		
		TElem (int idx, T val) : m_Val(val) {
			m_Idx = idx;
		}
		friend bool operator< (const TElem& a, const TElem& b) {
			return a.m_Idx < b.m_Idx;
		}
		friend bool operator> (const TElem& a, const TElem& b) {
			return a.m_Idx > b.m_Idx;
		}
		friend bool operator== (const TElem& a, const TElem& b) {
			return a.m_Idx == b.m_Idx;
		}
		friend ostream& operator<< (ostream& os, const TElem& print) {
			os << "[" << print.m_Idx << "] => " << print.m_Val;
			return os;
		}
		
		bool Equals (const TElem& b) const {
			bool ret = (this->m_Val == b.m_Val) && (this->m_Idx == b.m_Idx);
			cerr << m_Val << "==" << b.m_Val << ", " << m_Idx << "==" << b.m_Idx << " = " << ret << endl;
			return ret;
		}
		
		TElem& operator= (const TElem& b) {
			m_Idx = b.m_Idx;
			m_Val = b.m_Val;
			return *this;
		}
	};
	
	public:
		CSparseArray ( void );
		CSparseArray (const CSparseArray<T>& src);
		~CSparseArray ( void);
		CSparseArray<T>& Set (int idx, T val);
		CSparseArray<T>& Unset (int idx);
		bool IsSet (int idx) const;
		int Size ( void ) const;
		
		friend ostream& operator<< <> (ostream& os, const CSparseArray<T>& print);
		friend bool operator== <> (const CSparseArray<T>& a, const CSparseArray<T>& b);
		friend bool operator!= <> (const CSparseArray<T>& a, const CSparseArray<T>& b);
		
		T& operator[] (int x); //lvalue
		const T& operator[] (int x) const; //rvalue
		CSparseArray& operator= (const CSparseArray<T>& src);
 };

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>::CSparseArray (void) {
	m_Tree = new CBST<TElem>;
}
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>::CSparseArray (const CSparseArray<T>& src) {
	m_Tree = new CBST<TElem>;
	*m_Tree = *src.m_Tree;
}
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>::~CSparseArray (void) {
//	delete m_Tree;
}
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>& CSparseArray<T>::Set (int idx, T val) {
	m_Tree->Insert(TElem(idx, val));
	return *this;
}
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>& CSparseArray<T>::Unset (int idx) {
	m_Tree->Delete(TElem(idx, 0));
	return *this;
}
//-------------------------------------------------------------------------------------
template <class T>
bool CSparseArray<T>::IsSet (int idx) const {
	bool ret = (m_Tree->IsSet(TElem(idx, 0)) != NULL);
	return ret;
}
//-------------------------------------------------------------------------------------
template <class T>
int CSparseArray<T>::Size ( void ) const {
	return m_Tree->Size();
}
//-------------------------------------------------------------------------------------
template <class T>
ostream& operator<< (ostream& os, const CSparseArray<T>& print) {
	os << *print.m_Tree;
	return os;
}
//-------------------------------------------------------------------------------------
template <class T>
bool operator== (const CSparseArray<T>& a, const CSparseArray<T>& b) {
	if (a.Size() != b.Size()) return false;
	ostringstream sa, sb;
	sa << a;
	sb << b;
	return sa.str() == sb.str();
	//return (*a.m_Tree == *b.m_Tree);
//return true;
}
//-------------------------------------------------------------------------------------
template <class T>
bool operator!= (const CSparseArray<T>& a, const CSparseArray<T>& b) {
	return !(a == b);
}
//-------------------------------------------------------------------------------------
template <class T>
T& CSparseArray<T>::operator[] (int x) {
	TElem* ret = m_Tree->IsSet(TElem(x, 0));
	if (!ret) throw InvalidIndexException(x);
	return ret->m_Val;
}
//-------------------------------------------------------------------------------------
template <class T>
const T& CSparseArray<T>::operator[] (int x) const { //rvalue
	TElem* ret = m_Tree->IsSet(TElem(x, 0));
	if (!ret) throw InvalidIndexException(x);
	return ret->m_Val;
}
//-------------------------------------------------------------------------------------
template <class T>
CSparseArray<T>& CSparseArray<T>::operator= (const CSparseArray<T>& src) {
	if (&src == this) return *this;
	delete m_Tree;
	m_Tree = new CBST<TElem>;
	*m_Tree = *src.m_Tree;
	return *this;
}
