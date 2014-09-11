//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef INLINETAG_H
#define INLINETAG_H
#include "tagnode.h"
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

//! \brief Class represents inline tag.
class InlineTag : public TagNode {
protected:
	//void  print(ostream& os, string prefix="", string suffix="") const;
	void  printString	(ostream& os, string& toPrint, string prefix) const;
public:
				InlineTag 	(Tag * tag);
	void 	PushToStack (Stack<TagNode*>& stack);
};
#endif
