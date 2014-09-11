//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef DOCTYPETAG_H
#define DOCTYPETAG_H

#include "tagnode.h"
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

//! \brief Class represents tag <!DOCTYPE ...>
class DoctypeTag : public TagNode {
protected:
	string m_Content;	//!< doctype attributes

	//void 	print 			(ostream& os, string prefix = "", string suffix="") const;
	void 	printString(ostream& os, string& toPrint, string prefix) const;
public:
				DoctypeTag 	(string content);
	void 	PushToStack (Stack<TagNode*>& stack);
};
#endif
