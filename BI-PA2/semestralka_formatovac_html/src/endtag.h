//Author: Jakub Olejnik
//olejnjak@fit.cvut.cz

#ifndef ENDTAG_H
#define ENDTAG_H
#include "tagnode.h"

//! \brief Class represents ending tag.
class EndTag : public TagNode {
	//void 	print 			(ostream& os, string prefix="", string suffix="") const;
	void  printString	(ostream& os, string& toPrint, string prefix) const;
public:
				EndTag 			(string name);
	void 	PushToStack (Stack<TagNode*>& stack);
};
#endif
