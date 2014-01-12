static int ids = 0;

class CBTreePlus;
class CInnerNode;
class CLeafNode;
class CNode;

class CNode {
	friend class CBTreePlus;
	friend class CLeafNode;
	protected:
		int m, gamma, id;
		CKey * keys;
		
  public:
    CNode(int m);
    virtual bool isLeaf(void) const = 0;
    int getGamma(void) const { return gamma; }
    virtual CNode* getParent(void) const = 0;
    CKey& getKey(int index) const;
    int getID(void) const { return id; }
    void setID(int id) { this->id = id; }
    virtual ~CNode(void) { 	
    	delete [] keys; 
    }
    
    virtual int search (const CKey& key, CLeafNode ** val) const = 0;
    virtual void setParent (CInnerNode * parent) = 0;
};

class CInnerNode : public CNode {
	friend class CBTreePlus;
	friend class CLeafNode;
	protected:
		CNode ** childs;
		CInnerNode * parent;
		
		void shiftKeysRight(int idx);
		void shiftKeysLeft(int idx);
		void shiftKeysLeftNC(int idx);
		
  public:
    CInnerNode(int m, CInnerNode * parent = NULL);
    CNode* getChild(int index) const;
    bool isLeaf(void) const { return false; }
    CNode* getParent(void) const { return parent; } 
    ~CInnerNode(void) { for (int i = 0; i <= gamma; i++) delete childs[i]; delete [] childs; }
    
		int search (const CKey& key, CLeafNode ** val) const;
		void insert (const CKey& key, CNode * child);
		void setParent (CInnerNode * parent) { this->parent = parent; }
};


class CLeafNode : public CNode {
	friend class CBTreePlus;
	protected:
		CValue * values;
		CLeafNode * next;
		CInnerNode * parent;
		
		void shiftKeysRight (int idx);
		
  public:
    CLeafNode(int m, CInnerNode * parent = NULL, CLeafNode * next = NULL);
    CValue& getValue(int index) const;  
    CLeafNode* getNextLeaf(void) const { return next; }
    bool isLeaf(void) const { return true; }
    CNode* getParent(void) const { return parent; } 
    ~CLeafNode(void) { delete [] values; }
    
 		int search (const CKey& key, CLeafNode ** val) const;
 		void insert (const CKey& key, const CValue& val, int index = -1);
 		void setParent (CInnerNode * parent) { this->parent = parent; }
};

class CBTreePlus {
	CNode * root;
	int m;
	
	public:
		CBTreePlus(int m);
		bool bTreePlusInsert(const CKey& key, const CValue& val);
		int bTreePlusSearch(const CKey& key, CLeafNode** val);
		bool bTreePlusDelete(const CKey& key);
		CNode *getRoot(void) { return root; }
		~CBTreePlus(void) { delete root; }
		
		void setRoot (CNode * r) { root = r; }
};

CNode::CNode (int m) {
	this->m = m;
	gamma = 0;
	id = ids++;
	
	keys = new CKey[m+1];
}

CKey& CNode::getKey(int index) const {
	return keys[index-1]; //indexuji od 0
}

CLeafNode::CLeafNode(int m, CInnerNode * parent, CLeafNode * next) : CNode(m) {
	values = new CValue[m];
	this->parent = parent;
	this->next = next;
}

CValue& CLeafNode::getValue(int index) const {
	return values[index-1]; //indexuji od 0
}

int CLeafNode::search (const CKey& key, CLeafNode** val) const {
	for (int i = 0; i < gamma; i++) {
		if (keys[i] == key) { 
			*val = (CLeafNode*)this; 
			return i; 
		}
	}
	*val = (CLeafNode*)this;
	return -1;
}

void CLeafNode::insert (const CKey& key, const CValue& val, int index) {
	if (index != -1) {
		values[index] = val;
		return;
	}

	if (gamma == 0) {
		keys[0] = key;
		values[0] = val;
		gamma++;
		return; 
	}
	
	if (gamma == m-1) {
		CLeafNode * new_leaf = new CLeafNode (m, parent, next);
		next = new_leaf;
		
		int g = gamma;
		for (int i = m/2; i < gamma; i++) {
			new_leaf->insert(keys[i], values[i]);
			g--;
		}
		gamma = g;
		
		if (parent) {
			parent->insert(keys[gamma], new_leaf);
		}
		
		else {
			parent = new_leaf->parent = new CInnerNode(m);
			parent->childs[0] = this;
			parent->childs[1] = new_leaf;
			parent->gamma = 1;
			parent->keys[0] = keys[gamma];
		}		
		if (key >= keys[gamma]) new_leaf->insert(key, val);
		else insert(key, val);
	}
	
	else {
		for (int i = 0; i < gamma; i++) {
			if (key < keys[i]) {
				shiftKeysRight(i);

				keys[i] = key;
				values[i] = val;

				return;
			}
		}
		
		keys[gamma] = key;
		values[gamma++] = val;
	}
}

void CLeafNode::shiftKeysRight (int idx) {
	for (int i = gamma; i > idx; i--) {
		keys[i] = keys[i-1];
		values[i] = values[i-1];
	}
	gamma++;
}

CInnerNode::CInnerNode (int m, CInnerNode * parent) : CNode (m) {
	childs = new CNode*[m+1];
	this->parent = parent;
	for (int i = 0; i < m; i++) childs[i] = NULL;
}

CNode* CInnerNode::getChild(int index) const {
	return childs[index-1]; //indexuji od 0;
}

int CInnerNode::search (const CKey& key, CLeafNode** val) const {
	for (int i = 0; i < gamma; i++) {
		if (key >= keys[i]) continue;
		return childs[i]->search(key, val);
	}
	return childs[gamma]->search(key, val);
}

void CInnerNode::insert (const CKey& key, CNode * child) {
	if (gamma == m-1) {
		
		CInnerNode * new_node = new CInnerNode(m, parent);
		CKey propagate;

		for (int i = 0; i <= gamma; i++) {
			if (i == gamma || key < keys[i]) {
				for (int j = gamma; j > i; j--) {
					keys[j] = keys[j-1];
					childs[j+1] = childs[j];
				}
				
				keys[i] = key;
				childs[i+1] = child;				
				break;
			}
		}
		
		propagate = keys[m/2];
		for (int i = m/2; i < gamma; i++) keys[i] = keys[i+1];
		
		int g = gamma;
		for (int i = m/2; i <= g; i++) {
			if (i < g) {
				new_node->keys[new_node->gamma] = keys[i];
				new_node->childs[new_node->gamma++] = childs[i+1];
				childs[i+1]->setParent(new_node);
				gamma--;
			} else {
				new_node->childs[new_node->gamma] = childs[i+1];
				childs[i+1]->setParent(new_node);
			}
		}
		
		if (parent) parent->insert(propagate, new_node);
		else {
			parent = new_node->parent = new CInnerNode(m);
			parent->keys[0] = propagate;
			parent->childs[0] = this;
			parent->childs[1] = new_node;
			parent->gamma = 1;
		}
	}
	
	else {
		for (int i = 0; i < gamma; i++) {
			if (key < keys[i]) {
				shiftKeysRight(i);
				
				keys[i] = key;
				childs[i+1] = child;
				child->setParent(this);
				return;
			}
		}
		keys[gamma] = key;
		childs[gamma+++1] = child;
	}
}

void CInnerNode::shiftKeysRight(int idx) {
	for (int i = gamma; i > idx; i--) {
		keys[i] = keys[i-1];
		childs[i+1] = childs[i];
	}
	gamma++;
}

void CInnerNode::shiftKeysLeftNC(int idx) {
	for (int i = idx; i < gamma; i++) {
		keys[i] = keys[i+1];
	}
	gamma--;
}

CBTreePlus::CBTreePlus(int m) {
	this->m = m;
	root = new CLeafNode(m);
}

int CBTreePlus::bTreePlusSearch(const CKey& key, CLeafNode** val) {
	if (root)	{
		int r = root->search(key, val);
		if (r == -1) { *val = NULL; return r; }
		return r+1;
	}
	val = NULL;
	return -1;
}

bool CBTreePlus::bTreePlusInsert(const CKey& key, const CValue& val) {
	if (!root) {
		CLeafNode * l = new CLeafNode (m);
		l->insert(key, val);
		
		root = l;

		return true;
	}

	CLeafNode * leaf;
	int ret = root->search (key, &leaf);
	leaf->insert(key, val, ret);
	
	while (root->getParent()) root = root->getParent();

	if (ret == -1) return true;
	return false;
}

bool CBTreePlus::bTreePlusDelete(const CKey& key) {
	CLeafNode * d;
	if (root->search(key, &d) == -1) return false;

	CNode * r = root;
	while (!r->isLeaf()) r = ((CInnerNode*)r)->getChild(1);
	CLeafNode * l = (CLeafNode*)r;
	
	CBTreePlus * nt = new CBTreePlus(m);
	bool ret = false;
	
	while (l) {
		for (int i = 1; i <= l->getGamma(); i++) {
			CKey k = l->getKey(i);
			
			if (k == key) { ret = true; continue; }
			CValue v = l->getValue(i);
			nt->bTreePlusInsert(k, v);
		}
		l = l->getNextLeaf();
	}
	
	delete root;
	root = nt->getRoot();
	nt->setRoot(NULL);
	delete nt;
	return ret;
}
