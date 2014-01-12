template <class T>
class CSparseArray {
   public:
   	CSparseArray (void);
   	CSparseArray (const CSparseArray& src);
   	~CSparseArray (void);
		CSparseArray<T>& Set (int idx, T val);
		CSparseArray<T>& Unset (int idx);
		bool IsSet (int idx) const;
		int Size (void) const;
		
		template <class X>
		friend bool operator== (const CSparseArray<X>& left, const CSparseArray<X>& right);
		template <class X>
		friend bool operator!= (const CSparseArray<X>& left, const CSparseArray<X>& right);
		template <class X>
		friend ostream& operator<< (ostream& os, const CSparseArray<X>& print);
		
		T operator[] (int idx); //lvalue
		const T operator[] (int idx) const; //rvalue
		CSparseArray& operator= (const CSparseArray& src);
		
   protected:
    struct TElem {
       TElem * m_Next;
       int m_Idx; 
       T m_Val; 
       TElem(int idx, const T& val, TElem * next = NULL) : m_Val(val) { m_Next = next; m_Idx = idx;};
    };
    TElem * m_First;
    TElem * m_Last; 
    int m_Size;

		void free (void);
		void copy (const CSparseArray& src);
		void init (void);
		TElem * getIfSet(int idx) const;
};

template <class T>
CSparseArray<T>::CSparseArray(void) {
	init();
}

template <class T>
CSparseArray<T>::~CSparseArray(void) {
	free();
}

template <class T>
void CSparseArray<T>::free(void) {
	while(m_First) {
		TElem * current = m_First;
		m_First = m_First->m_Next;
		delete current;
	}
}

template <class T>
bool CSparseArray<T>::IsSet(int idx) const {
	return (getIfSet(idx)?true:false);
}

template <class T>
CSparseArray<T>& CSparseArray<T>::Set (int idx, T val) {
	//empty array
	if (!m_First || (idx < m_First->m_Idx)) { //insert at the beginning
		m_First =  new TElem(idx, val, m_First);
		if (!m_Last) m_Last = m_First;
		m_Size++;
		return *this;
	}//=> m_Last is set
	//the biggest idx
	if (idx > m_Last->m_Idx) { //insert at the end
		m_Last = m_Last->m_Next = new TElem(idx, val);
		m_Size++;
		return *this;
	}
	if (idx == m_Last->m_Idx) { //update at the end
		m_Last->m_Val = val;
		return *this;
	}
	
	TElem * previous = NULL, * current = m_First;
	while (idx > current->m_Idx) {
		previous = current;
		current = current->m_Next;
	}
	if (current->m_Idx == idx) {
		current->m_Val = val;
		return *this;
	}
	m_Size++;
	previous->m_Next = new TElem(idx, val, previous->m_Next);
	return *this;
}

template <class T>
CSparseArray<T>& CSparseArray<T>::Unset (int idx) {
	if (!m_First) return *this; //=> m_Last is set
	if (m_Last->m_Idx < idx) return *this; //idx is greater then max stored idx
	TElem * current = m_First, * previous = NULL;
	while (current) {
		if (current->m_Idx == idx) { //found idx
			if(!previous) m_First = current->m_Next;
			else previous->m_Next = current->m_Next;
			
			if (current->m_Next == NULL) m_Last = previous;
			
			m_Size--;
			delete current;
			return *this;
		}
		else if (current->m_Idx > idx) return *this;
		
		previous = current;
		current = current->m_Next;
	}
	return *this;
}



template <class T>
ostream& operator<< (ostream& os, const CSparseArray<T>& print) {
//	os << print.m_First << " - " << print.m_Last << " " << print.m_Last->m_Next;
	os << "{";
	typename CSparseArray<T>::TElem * current = print.m_First;
	if (current) {
		os << " [" << current->m_Idx << "]" << " => " << current->m_Val;
//		os << " [" << current->m_Idx << "]" << " => " << current->m_Val << " - " << current;
		current = current->m_Next;
	}
	while (current) {
		os << ", [" << current->m_Idx << "]" << " => " << current->m_Val;
		//os << " [" << current->m_Idx << "]" << " => " << current->m_Val << " - " << current;
		current = current->m_Next;
	}
	os << " }";
	return os;
}

template <class T>
typename CSparseArray<T>::TElem * CSparseArray<T>::getIfSet (int idx) const {
	TElem * current = m_First;
	
	while (current) {
		if (current->m_Idx == idx) return current;
		else if (current->m_Idx > idx) return NULL;
		current = current->m_Next;
	}
	
	return NULL;
}

template <class T>
T CSparseArray<T>::operator[] (int idx) { //lvalue
	TElem * get = getIfSet(idx);
	if (!get) throw InvalidIndexException(idx);
	return get->m_Val;
}

template <class T>
const T CSparseArray<T>::operator[] (int idx) const { //rvalue
	TElem * get = getIfSet(idx);
	if (!get) throw InvalidIndexException(idx);
	return get->m_Val;
}

template <class T>
int CSparseArray<T>::Size(void) const {
	return m_Size;
}

template <class X>
bool operator== (const CSparseArray<X>& left, const CSparseArray<X>& right) {
	typename CSparseArray<X>::TElem * current, *current2;
	current = left.m_First;
	current2 = right.m_First;
	
	while (current && current2) {
		if (current->m_Idx != current->m_Idx) return false;
		if (current->m_Val != current2->m_Val) return false;
				
		current = current->m_Next;
		current2 = current2->m_Next;
	}	
	if (!current && !current2) return true;
	return false;
}

template <class X>
bool operator!= (const CSparseArray<X>& left, const CSparseArray<X>& right) {
	return !(left==right);
}

template <class T>
void CSparseArray<T>::copy (const CSparseArray& src) {
	init();
	typename CSparseArray<T>::TElem * current = src.m_First;
	
	while (current) {
		Set(current->m_Idx, current->m_Val);
		current = current->m_Next;
	}
}

template <class T>
CSparseArray<T>::CSparseArray (const CSparseArray& src) {
	copy(src);
}

template <class T>
CSparseArray<T>& CSparseArray<T>::operator= (const CSparseArray<T>& src) {
	if (&src == this) return *this;
	free();
	copy(src);
	return *this;
}

template <class T>
void CSparseArray<T>::init (void) {
	m_First = NULL;
	m_Last = NULL;
	m_Size = 0;
}
